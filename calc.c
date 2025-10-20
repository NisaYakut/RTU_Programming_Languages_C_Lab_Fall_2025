// Nisa Yakut 123456
// Compile with: gcc -O2 -Wall -Wextra -std=c17 -o calc calc.c
//
// This program reads and calculates arithmetic expressions from text files.
// It supports +, -, *, / and parentheses. 
// If there is any mistake in the input, it writes ERROR with the position number.
// I used a simple recursive parser because it is easier to understand and follow.
// I tried to make the program clean, clear and easy to test.
// It can work for one file or all .txt files in a folder (with the -d option).

#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <math.h>
#include <dirent.h>
#include <sys/stat.h>

#define NAME_STR     "Nisa"
#define LASTNAME_STR "Yakut"
#define STUDENTID_STR "123456"

typedef enum {
    T_EOF=0, T_NUM, T_PLUS, T_MINUS, T_STAR, T_SLASH, T_LPAREN, T_RPAREN
} TokType;

typedef struct {
    TokType type;
    const char *start;
    size_t len;
    long pos1b;
    double num;
    int is_float;
} Token;

typedef struct {
    const char *src;
    size_t n;
    size_t i;
    long pos1b;
    Token cur;
    int error_set;
    long error_pos;
} Lexer;

static void set_error(Lexer *L, long pos) {
    if (!L->error_set) {
        L->error_set = 1;
        L->error_pos = pos < 1 ? 1 : pos;
    }
}

// check if # is first non-space on a line
static int at_line_start_nonspace(const char *s, size_t idx) {
    size_t j = idx;
    while (j > 0 && s[j-1] != '\n') j--;
    for (size_t k = j; k < idx; k++) {
        if (!(s[k]==' ' || s[k]=='\t' || s[k]=='\r')) return 0;
    }
    return 1;
}

// read next token
static void next_token(Lexer *L) {
    if (L->error_set) { L->cur.type = T_EOF; return; }
    const char *s = L->src;
    size_t n = L->n;

    while (L->i < n) {
        char c = s[L->i];
        if (isspace((unsigned char)c)) {
            L->i++; L->pos1b++;
        } else if (c=='#' && at_line_start_nonspace(s, L->i)) {
            while (L->i < n && s[L->i] != '\n') { L->i++; L->pos1b++; }
        } else break;
    }

    if (L->i >= n) {
        L->cur = (Token){.type=T_EOF, .start=s+L->i, .len=0, .pos1b=L->pos1b};
        return;
    }

    char c = s[L->i];
    long pos = L->pos1b;

    // numbers
    if (isdigit((unsigned char)c) || c=='.') {
        const char *startp = s+L->i;
        char *endp = NULL;
        errno = 0;
        double val = strtod(startp, &endp);
        if (endp == startp) {
            set_error(L, pos);
            return;
        }
        size_t used = (size_t)(endp - startp);
        int is_float = 0;
        for (size_t k=0;k<used;k++) {
            char ch = startp[k];
            if (ch=='.' || ch=='e' || ch=='E') { is_float = 1; break; }
        }
        L->i += used; L->pos1b += (long)used;
        L->cur = (Token){.type=T_NUM, .start=startp, .len=used, .pos1b=pos, .num=val, .is_float=is_float};
        return;
    }

    // single char tokens
    switch (c) {
        case '+': L->i++; L->pos1b++; L->cur=(Token){T_PLUS,s+L->i-1,1,pos}; return;
        case '-': L->i++; L->pos1b++; L->cur=(Token){T_MINUS,s+L->i-1,1,pos}; return;
        case '*': L->i++; L->pos1b++; L->cur=(Token){T_STAR,s+L->i-1,1,pos}; return;
        case '/': L->i++; L->pos1b++; L->cur=(Token){T_SLASH,s+L->i-1,1,pos}; return;
        case '(': L->i++; L->pos1b++; L->cur=(Token){T_LPAREN,s+L->i-1,1,pos}; return;
        case ')': L->i++; L->pos1b++; L->cur=(Token){T_RPAREN,s+L->i-1,1,pos}; return;
        case '\n': L->i++; L->pos1b++; next_token(L); return;
        default: set_error(L, pos); return;
    }
}

typedef struct { double val; int is_float; } NumVal;
static NumVal make_num(double v, int f){ NumVal r={v,f}; return r; }

static NumVal parse_expr(Lexer *L);

static NumVal parse_primary(Lexer *L) {
    if (L->cur.type == T_NUM) {
        NumVal v = make_num(L->cur.num, L->cur.is_float);
        next_token(L);
        return v;
    }
    if (L->cur.type == T_LPAREN) {
        next_token(L);
        NumVal inner = parse_expr(L);
        if (L->cur.type != T_RPAREN) { set_error(L, L->pos1b); return inner; }
        next_token(L);
        return inner;
    }
    set_error(L, L->cur.pos1b);
    return make_num(0,1);
}

static NumVal parse_unary(Lexer *L) {
    if (L->cur.type == T_PLUS) { next_token(L); return parse_unary(L); }
    if (L->cur.type == T_MINUS) {
        next_token(L);
        NumVal v = parse_unary(L);
        return make_num(-v.val, 1);
    }
    return parse_primary(L);
}

static NumVal parse_factor(Lexer *L) { return parse_unary(L); }

static NumVal parse_term(Lexer *L) {
    NumVal left = parse_factor(L);
    while (!L->error_set && (L->cur.type==T_STAR || L->cur.type==T_SLASH)) {
        TokType op = L->cur.type;
        long pos = L->cur.pos1b;
        next_token(L);
        Token rhs_tok = L->cur;
        NumVal right = parse_factor(L);
        if (op == T_STAR) {
            double v = left.val * right.val;
            int isf = left.is_float || right.is_float || floor(v)!=v;
            left = make_num(v, isf);
        } else {
            if (right.val == 0.0) { set_error(L, rhs_tok.pos1b); return left; }
            double v = left.val / right.val;
            left = make_num(v, 1);
        }
    }
    return left;
}

static NumVal parse_expr(Lexer *L) {
    NumVal left = parse_term(L);
    while (!L->error_set && (L->cur.type==T_PLUS || L->cur.type==T_MINUS)) {
        TokType op = L->cur.type;
        next_token(L);
        NumVal right = parse_term(L);
        double v = (op==T_PLUS) ? (left.val + right.val) : (left.val - right.val);
        int isf = left.is_float || right.is_float || floor(v)!=v;
        left = make_num(v, isf);
    }
    return left;
}

static int is_int(double x) { return fabs(x - round(x)) < 1e-12; }

static char *read_file(const char *path, size_t *out_n) {
    FILE *f = fopen(path, "rb");
    if (!f) return NULL;
    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    rewind(f);
    char *buf = malloc(size + 1);
    fread(buf, 1, size, f);
    fclose(f);
    buf[size] = '\0';
    if (out_n) *out_n = size;
    return buf;
}

static int mk_outdir(const char *path) {
    struct stat st;
    if (stat(path, &st)==0 && S_ISDIR(st.st_mode)) return 0;
    return mkdir(path, 0775);
}

static void base_name_noext(const char *in, char *out, size_t cap) {
    const char *p = strrchr(in, '/');
    p = p ? p+1 : in;
    const char *dot = strrchr(p, '.');
    size_t cut = dot ? (size_t)(dot - p) : strlen(p);
    if (cut >= cap) cut = cap - 1;
    memcpy(out, p, cut); out[cut] = '\0';
}

static int ends_with(const char *s, const char *suf) {
    size_t ns = strlen(s), nf = strlen(suf);
    return nf<=ns && strcmp(s+ns-nf,suf)==0;
}

static int skip_output_file(const char *name) {
    return ends_with(name, "_output.txt");
}

static int process_one_file(const char *inpath, const char *outdir) {
    size_t n=0;
    char *buf = read_file(inpath, &n);
    if (!buf) { fprintf(stderr, "File read error: %s\n", inpath); return 1; }

    Lexer L={.src=buf,.n=n,.i=0,.pos1b=1,.error_set=0,.error_pos=0};
    next_token(&L);
    NumVal v = parse_expr(&L);
    if (!L.error_set && L.cur.type!=T_EOF) set_error(&L, L.cur.pos1b);

    char base[256]; base_name_noext(inpath, base, sizeof(base));
    char outfile[512];
    snprintf(outfile,sizeof(outfile),"%s/%s_%s_%s_%s.txt",
             outdir,base,NAME_STR,LASTNAME_STR,STUDENTID_STR);

    FILE *fo = fopen(outfile, "w");
    if (!fo) { fprintf(stderr,"Cannot open output: %s\n",outfile); free(buf); return 1; }

    if (L.error_set) fprintf(fo,"ERROR:%ld\n",L.error_pos);
    else if (is_int(v.val)) fprintf(fo,"%lld\n",(long long)round(v.val));
    else fprintf(fo,"%.15g\n",v.val);

    fclose(fo); free(buf); return 0;
}

int main(int argc, char **argv) {
    const char *indir=NULL,*outdir_opt=NULL,*single=NULL;
    for(int i=1;i<argc;i++){
        if(!strcmp(argv[i],"-d")||!strcmp(argv[i],"--dir")) indir=argv[++i];
        else if(!strcmp(argv[i],"-o")||!strcmp(argv[i],"--output-dir")) outdir_opt=argv[++i];
        else single=argv[i];
    }
    if(!indir && !single){
        printf("Usage: %s [-d DIR|--dir DIR] [-o OUTDIR|--output-dir OUTDIR] input.txt\n", argv[0]);
        return 2;
    }

    char outdir[256];
    if(outdir_opt) snprintf(outdir,sizeof(outdir),"%s",outdir_opt);
    else {
        char base[128]; const char *src=indir?indir:single;
        base_name_noext(src, base, sizeof(base));
        snprintf(outdir,sizeof(outdir),"%s_%s_%s",base,NAME_STR,STUDENTID_STR);
    }
    mk_outdir(outdir);

    if(indir){
        DIR *d = opendir(indir);
        if(!d){ fprintf(stderr,"Cannot open dir: %s\n", indir); return 3; }
        struct dirent *de;
        while((de=readdir(d))){
            if(de->d_name[0]=='.') continue;
            if(!ends_with(de->d_name,".txt")) continue;
            if(skip_output_file(de->d_name)) continue;
            char path[512]; snprintf(path,sizeof(path),"%s/%s",indir,de->d_name);
            process_one_file(path,outdir);
        }
        closedir(d);
    } else {
        process_one_file(single, outdir);
    }
    return 0;
}
