
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <md5.h>
#include <ctype.h>
#include <string.h>
#include <malloc.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/file.h>

/*
 * We redefine the names to make it look nice...
 */

#define VERSION "2.0"
#define MAXSIZE (1024*1024*10)

/* The fields... */
#define CTM_F_MASK		0xff
#define CTM_F_Name		0x01
#define CTM_F_Uid		0x02
#define CTM_F_Gid		0x03
#define CTM_F_Mode		0x04
#define CTM_F_MD5		0x05
#define CTM_F_Count		0x06
#define CTM_F_Bytes		0x07

/* The qualifiers... */
#define CTM_Q_MASK		0xff00
#define CTM_Q_Name_File		0x0100
#define CTM_Q_Name_Dir		0x0200
#define CTM_Q_Name_New		0x0400
#define CTM_Q_MD5_After		0x0100
#define CTM_Q_MD5_Before	0x0200
#define CTM_Q_MD5_Chunk		0x0400

struct CTM_Syntax {
    char	*Key;
    int		*List;
    };

extern struct CTM_Syntax Syntax[];

#define Malloc malloc
#define Free free

#ifndef EXTERN
#  define EXTERN extern
#endif
EXTERN u_char *Version;
EXTERN u_char *Name;
EXTERN u_char *Nbr;
EXTERN u_char *TimeStamp;
EXTERN u_char *Prefix;
EXTERN u_char *FileName;
EXTERN u_char *BaseDir;
EXTERN u_char *TmpDir;
EXTERN int Verbose;

/* 
 * Paranoid -- Just in case they should be after us...
 *  0 not at all.
 *  1 normal.
 *  2 somewhat.
 *  3 you bet!.
 *
 * Verbose -- What to tell mom...
 *  0 Nothing which wouldn't surprise.
 *  1 Normal. 
 *  2 Show progress '.'.
 *  3 Show progress names, and actions.
 *  4 even more...
 *  and so on
 */
EXTERN int Paranoid;


char * String(char *s);
void Fatal_(int ln, char *fn, char *kind);
#define Fatal(foo) Fatal_(__LINE__,__FILE__,foo)
#define Assert() Fatal_(__LINE__,__FILE__,"Assert failed.")
#define WRONG {Assert(); return 1;}

u_char * Ffield(FILE *fd, MD5_CTX *ctx,u_char term);

int Fbytecnt(FILE *fd, MD5_CTX *ctx, u_char term);

u_char * Fdata(FILE *fd, int u_chars, MD5_CTX *ctx);

#define GETFIELD(p,q) if(!((p)=Ffield(fd,&ctx,(q)))) return 1
#define GETFIELDCOPY(p,q) if(!((p)=Ffield(fd,&ctx,(q)))) return 1; else p=String(p)
#define GETBYTECNT(p,q) if(0 >((p)= Fbytecnt(fd,&ctx,(q)))) return 1
#define GETDATA(p,q) if(!((p) = Fdata(fd,(q),&ctx))) return 1

int Pass1(FILE *fd);
int Pass2(FILE *fd);
int Pass3(FILE *fd);

