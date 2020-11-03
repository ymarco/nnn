/*
 * BSD 2-Clause License
 *
 * Copyright (C) 2014-2016, Lazaros Koromilas <lostd@2f30.org>
 * Copyright (C) 2014-2016, Dimitris Papastamos <sin@2f30.org>
 * Copyright (C) 2016-2020, Arun Prakash Jana <engineerarun@gmail.com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#pragma once

#include <curses.h>
#include <sys/stat.h>
#include <ftw.h>

#ifdef PCRE
#include <pcre.h>
#else
#include <regex.h>
#endif

#define CONTROL(c) ((c) & 0x1f)

#ifndef ESC
#define ESC (27)
#endif

#ifndef DEL
#define DEL (127)
#endif

/* Supported actions */
enum action {
	SEL_BACK = 1,
	SEL_OPEN,
	SEL_NAV_IN,
	SEL_NEXT,
	SEL_PREV,
	SEL_PGDN,
	SEL_PGUP,
	SEL_CTRL_D,
	SEL_CTRL_U,
	SEL_HOME,
	SEL_END,
	SEL_FIRST,
	SEL_CDHOME,
	SEL_CDBEGIN,
	SEL_CDLAST,
	SEL_CDROOT,
	SEL_BOOKMARK,
	SEL_REMOTE,
	SEL_CYCLE,
	SEL_CYCLER,
	SEL_CTX1,
	SEL_CTX2,
	SEL_CTX3,
	SEL_CTX4,
#ifdef CTX8
	SEL_CTX5,
	SEL_CTX6,
	SEL_CTX7,
	SEL_CTX8,
#endif
	SEL_MARK,
	SEL_FLTR,
	SEL_MFLTR,
	SEL_HIDDEN,
	SEL_DETAIL,
	SEL_STATS,
	SEL_CHMODX,
	SEL_ARCHIVE,
	SEL_SORT,
	SEL_REDRAW,
	SEL_SEL,
	SEL_SELMUL,
	SEL_SELALL,
	SEL_SELEDIT,
	SEL_CP,
	SEL_MV,
	SEL_CPMVAS,
	SEL_RM,
	SEL_OPENWITH,
	SEL_NEW,
	SEL_RENAME,
	SEL_RENAMEMUL,
	SEL_UMOUNT,
	SEL_HELP,
	SEL_AUTONEXT,
	SEL_EDIT,
	SEL_PLUGIN,
	SEL_SHELL,
	SEL_LAUNCH,
	SEL_RUNCMD,
	SEL_LOCK,
	SEL_SESSIONS,
	SEL_EXPORT,
	SEL_TIMETYPE,
	SEL_QUITCTX,
	SEL_QUITCD,
	SEL_QUIT,
	SEL_QUITFAIL,
#ifndef NOFIFO
	SEL_FIFO,
#endif
#ifndef NOMOUSE
	SEL_CLICK,
#endif
};

/* Associate a pressed key to an action */
struct key {
	int sym;         /* Key pressed */
	enum action act; /* Action */
};

static struct key bindings[] = {
	/* Back */
	{ KEY_LEFT,       SEL_BACK },
	{ 'h',            SEL_BACK },
	/* Inside or select */
	{ KEY_ENTER,      SEL_OPEN },
	{ '\r',           SEL_OPEN },
	/* Pure navigate inside */
	{ KEY_RIGHT,      SEL_NAV_IN },
	{ 'l',            SEL_NAV_IN },
	/* Next */
	{ 'j',            SEL_NEXT },
	{ KEY_DOWN,       SEL_NEXT },
	/* Previous */
	{ 'k',            SEL_PREV },
	{ KEY_UP,         SEL_PREV },
	/* Page down */
	{ KEY_NPAGE,      SEL_PGDN },
	/* Page up */
	{ KEY_PPAGE,      SEL_PGUP },
	/* Ctrl+D */
	{ CONTROL('D'),   SEL_CTRL_D },
	/* Ctrl+U */
	{ CONTROL('U'),   SEL_CTRL_U },
	/* First entry */
	{ KEY_HOME,       SEL_HOME },
	{ 'g',            SEL_HOME },
	{ CONTROL('A'),   SEL_HOME },
	/* Last entry */
	{ KEY_END,        SEL_END },
	{ 'G',            SEL_END },
	{ CONTROL('E'),   SEL_END },
	/* Go to first file */
	{ '\'',           SEL_FIRST },
	/* HOME */
	{ '~',            SEL_CDHOME },
	/* Initial directory */
	{ '@',            SEL_CDBEGIN },
	/* Last visited dir */
	{ '-',            SEL_CDLAST },
	/* Go to / */
	{ '`',            SEL_CDROOT },
	/* Leader key */
	{ 'b',            SEL_BOOKMARK },
	{ CONTROL('_'),   SEL_BOOKMARK },
	/* Connect to server over SSHFS */
	{ 'c',            SEL_REMOTE },
	/* Cycle contexts in forward direction */
	{ '\t',           SEL_CYCLE },
	/* Cycle contexts in reverse direction */
	{ KEY_BTAB,       SEL_CYCLER },
	/* Go to/create context N */
	{ '1',            SEL_CTX1 },
	{ '2',            SEL_CTX2 },
	{ '3',            SEL_CTX3 },
	{ '4',            SEL_CTX4 },
#ifdef CTX8
	{ '5',            SEL_CTX5 },
	{ '6',            SEL_CTX6 },
	{ '7',            SEL_CTX7 },
	{ '8',            SEL_CTX8 },
#endif
	/* Mark a path to visit later */
	{ ',',            SEL_MARK },
	/* Filter */
	{ '/',            SEL_FLTR },
	/* Toggle filter mode */
	{ CONTROL('N'),   SEL_MFLTR },
	/* Toggle hide .dot files */
	{ '.',            SEL_HIDDEN },
	/* Detailed listing */
	{ 'd',            SEL_DETAIL },
	/* File details */
	{ 'f',            SEL_STATS },
	{ CONTROL('F'),   SEL_STATS },
	/* Toggle executable status */
	{ '*',            SEL_CHMODX },
	/* Create archive */
	{ 'z',            SEL_ARCHIVE },
	/* Sort toggles */
	{ 't',            SEL_SORT },
	{ CONTROL('T'),   SEL_SORT },
	/* Redraw window */
	{ CONTROL('L'),   SEL_REDRAW },
	/* Select current file path */
	{ CONTROL('J'),   SEL_SEL },
	{ ' ',            SEL_SEL },
	/* Toggle select multiple files */
	{ 'm',            SEL_SELMUL },
	{ CONTROL(' '),   SEL_SELMUL },
	/* Select all files in current dir */
	{ 'a',            SEL_SELALL },
	/* List, edit selection */
	{ 'E',            SEL_SELEDIT },
	/* Copy from selection buffer */
	{ 'p',            SEL_CP },
	{ CONTROL('P'),   SEL_CP },
	/* Move from selection buffer */
	{ 'v',            SEL_MV },
	{ CONTROL('V'),   SEL_MV },
	/* Copy/move from selection buffer and rename */
	{ 'w',            SEL_CPMVAS },
	{ CONTROL('W'),   SEL_CPMVAS },
	/* Delete from selection buffer */
	{ 'x',            SEL_RM },
	{ CONTROL('X'),   SEL_RM },
	/* Open in a custom application */
	{ 'o',            SEL_OPENWITH },
	{ CONTROL('O'),   SEL_OPENWITH },
	/* Create a new file */
	{ 'n',            SEL_NEW },
	/* Show rename prompt */
	{ CONTROL('R'),   SEL_RENAME },
	/* Rename contents of current dir */
	{ 'r',            SEL_RENAMEMUL },
	/* Disconnect a SSHFS mount point */
	{ 'u',            SEL_UMOUNT },
	/* Show help */
	{ '?',            SEL_HELP },
	/* Quit a context */
	{ '+',            SEL_AUTONEXT },
	/* Edit in EDITOR */
	{ 'e',            SEL_EDIT },
	/* Run a plugin */
	{ ';',            SEL_PLUGIN },
	/* Run command */
	{ '!',            SEL_SHELL },
	{ CONTROL(']'),   SEL_SHELL },
	/* Launcher */
	{ '=',            SEL_LAUNCH },
	/* Run a command */
	{ ']',            SEL_RUNCMD },
	/* Lock screen */
	{ '0',            SEL_LOCK },
	/* Manage sessions */
	{ 's',            SEL_SESSIONS },
	/* Export list */
	{ '>',            SEL_EXPORT },
	/* Set time type */
	{ 'T',            SEL_TIMETYPE },
	/* Quit a context */
	{ 'q',            SEL_QUITCTX },
	/* Change dir on quit */
	{ CONTROL('G'),   SEL_QUITCD },
	/* Quit */
	{ CONTROL('Q'),   SEL_QUIT },
	/* Quit with an error code */
	{ 'Q',            SEL_QUITFAIL },
#ifndef NOFIFO
	/* Send hovered path to NNN_FIFO */
	{ ESC,            SEL_FIFO },
#endif
#ifndef NOMOUSE
	{ KEY_MOUSE,      SEL_CLICK },
#endif
};


/* TYPE DEFINITIONS */
typedef unsigned long ulong;
typedef unsigned int uint;
typedef unsigned char uchar;
typedef unsigned short ushort;
typedef long long ll;
typedef unsigned long long ull;

/*
 * NAME_MAX and PATH_MAX may not exist, e.g. with dirent.c_name being a
 * flexible array on Illumos. Use somewhat accomodating fallback values.
 */
#ifndef NAME_MAX
#define NAME_MAX 255
#endif

#ifndef PATH_MAX
#define PATH_MAX 4096
#endif

#define REGEX_MAX 48
/* STRUCTURES */

/* Directory entry */
typedef struct entry {
	char *name;
	time_t t;
	off_t size;
	blkcnt_t blocks; /* number of 512B blocks allocated */
	mode_t mode;
#ifndef NOUG
	uid_t uid;
	gid_t gid;
#endif
	ushort nlen; /* Length of file name, including null byte */
	uchar flags; /* Flags specific to the file */
} *pEntry;

/*
 * Settings
 * NOTE: update default values if changing order
 */
typedef struct {
	uint filtermode : 1;  /* Set to enter filter mode */
	uint timeorder  : 1;  /* Set to sort by time */
	uint sizeorder  : 1;  /* Set to sort by file size */
	uint apparentsz : 1;  /* Set to sort by apparent size (disk usage) */
	uint blkorder   : 1;  /* Set to sort by blocks used (disk usage) */
	uint extnorder  : 1;  /* Order by extension */
	uint showhidden : 1;  /* Set to show hidden files */
	uint reserved0  : 1;
	uint showdetail : 1;  /* Clear to show lesser file info */
	uint ctxactive  : 1;  /* Context active or not */
	uint reverse    : 1;  /* Reverse sort */
	uint version    : 1;  /* Version sort */
	uint reserved1  : 1;
	/* The following settings are global */
	uint curctx     : 3;  /* Current context number */
	uint prefersel  : 1;  /* Prefer selection over current, if exists */
	uint reserved2  : 1;
	uint nonavopen  : 1;  /* Open file on right arrow or `l` */
	uint autoselect : 1;  /* Auto-select dir in type-to-nav mode */
	uint cursormode : 1;  /* Move hardware cursor with selection */
	uint useeditor  : 1;  /* Use VISUAL to open text files */
	uint reserved3  : 3;
	uint regex      : 1;  /* Use regex filters */
	uint x11        : 1;  /* Copy to system clipboard and show notis */
	uint timetype   : 2;  /* Time sort type (0: access, 1: change, 2: modification) */
	uint cliopener  : 1;  /* All-CLI app opener */
	uint waitedit   : 1;  /* For ops that can't be detached, used EDITOR */
	uint rollover   : 1;  /* Roll over at edges */
} settings;


/* Contexts or workspaces */
typedef struct {
	char c_path[PATH_MAX]; /* Current dir */
	char c_last[PATH_MAX]; /* Last visited dir */
	char c_name[NAME_MAX + 1]; /* Current file name */
	char c_fltr[REGEX_MAX]; /* Current filter */
	settings c_cfg; /* Current configuration */
	char* pnamebuf;
	struct entry *pdents;
	int ndents;
	int (*nftw_fn)(const char *fpath, const struct stat *sb, int typeflag, struct FTW *ftwbuf);
	uint color; /* Color code for directories */
} context;

#define MIN_DISPLAY_COLS ((CTX_MAX * 2) + 2) /* Two chars for [ and ] */
#define LONG_SIZE sizeof(ulong)
#define ARCHIVE_CMD_LEN 16
#define BLK_SHIFT_512 9

/* Detect hardlinks in du */
#define HASH_BITS (0xFFFFFF)
#define HASH_OCTETS (HASH_BITS >> 6) /* 2^6 = 64 */

/* Entry flags */
#define DIR_OR_LINK_TO_DIR 0x01
#define HARD_LINK 0x02
#define SYM_ORPHAN 0x04
#define FILE_MISSING 0x08
#define FILE_SELECTED 0x10

/* Macros to define process spawn behaviour as flags */
#define F_NONE    0x00  /* no flag set */
#define F_MULTI   0x01  /* first arg can be combination of args; to be used with F_NORMAL */
#define F_NOWAIT  0x02  /* don't wait for child process (e.g. file manager) */
#define F_NOTRACE 0x04  /* suppress stdout and strerr (no traces) */
#define F_NORMAL  0x08  /* spawn child process in non-curses regular CLI mode */
#define F_CONFIRM 0x10  /* run command - show results before exit (must have F_NORMAL) */
#define F_CHKRTN  0x20  /* wait for user prompt if cmd returns failure status */
#define F_CLI     (F_NORMAL | F_MULTI)
#define F_SILENT  (F_CLI | F_NOTRACE)

/* Version compare macros */
/*
 * states: S_N: normal, S_I: comparing integral part, S_F: comparing
 *         fractional parts, S_Z: idem but with leading Zeroes only
 */
#define S_N 0x0
#define S_I 0x3
#define S_F 0x6
#define S_Z 0x9

/* result_type: VCMP: return diff; VLEN: compare using len_diff/diff */
#define VCMP 2
#define VLEN 3

/* Volume info */
#define FREE 0
#define CAPACITY 1

/* STRUCTURES */

/* Key-value pairs from env */
typedef struct {
	int key;
	int off;
} kv;

typedef struct {
#ifdef PCRE
	const pcre *pcrex;
#else
	const regex_t *regex;
#endif
	const char *str;
} fltrexp_t;


extern settings default_cfg;

context *make_context(context *pctx, char* path, settings cfg);
void free_context(context *pctx);
void clean_context(void *pctx);
int dentfind(context *pctx, const char *fname);
char *visit_parent(char *path, char *newpath);
size_t mkpath(const char *dir, const char *name, char *out);
int set_sort_flags(context *pctx, int r);
void populate(context *pctx);
char* printent(char *dst, const struct entry *ent);
