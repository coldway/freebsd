/*
 * Copyright (c) 1999 Cameron Grant <gandalf@vilnya.demon.co.uk>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * $FreeBSD$
 */

struct pcmchan_caps {
	u_int32_t minspeed, maxspeed;
	u_int32_t *fmtlist;
	u_int32_t caps;
};

#define	CHN_NAMELEN	32
struct pcm_channel {
	kobj_t methods;

	struct pcm_feeder *feeder;
	u_int32_t align;

	int volume;
	u_int32_t speed;
	u_int32_t format;
	u_int32_t flags;
	u_int32_t feederflags;
	u_int32_t blocks;

	int direction;
	struct snd_dbuf *bufhard, *bufsoft;
	struct snddev_info *parent;
	void *devinfo;
	char name[CHN_NAMELEN];
	void *lock;
};

#include "channel_if.h"

int chn_reinit(struct pcm_channel *c);
int chn_write(struct pcm_channel *c, struct uio *buf);
int chn_read(struct pcm_channel *c, struct uio *buf);
u_int32_t chn_start(struct pcm_channel *c, int force);
int chn_sync(struct pcm_channel *c, int threshold);
int chn_flush(struct pcm_channel *c);
int chn_poll(struct pcm_channel *c, int ev, struct proc *p);

int chn_init(struct pcm_channel *c, void *devinfo, int dir);
int chn_kill(struct pcm_channel *c);
int chn_setdir(struct pcm_channel *c, int dir);
int chn_reset(struct pcm_channel *c, u_int32_t fmt);
int chn_setvolume(struct pcm_channel *c, int left, int right);
int chn_setspeed(struct pcm_channel *c, int speed);
int chn_setformat(struct pcm_channel *c, u_int32_t fmt);
int chn_setblocksize(struct pcm_channel *c, int blkcnt, int blksz);
int chn_trigger(struct pcm_channel *c, int go);
int chn_getptr(struct pcm_channel *c);
struct pcmchan_caps *chn_getcaps(struct pcm_channel *c);
u_int32_t chn_getformats(struct pcm_channel *c);

void chn_resetbuf(struct pcm_channel *c);
void chn_intr(struct pcm_channel *c);
int chn_wrfeed(struct pcm_channel *c);
int chn_rdfeed(struct pcm_channel *c);
int chn_abort(struct pcm_channel *c);

void chn_wrupdate(struct pcm_channel *c);
void chn_rdupdate(struct pcm_channel *c);

#define CHN_LOCK(c) mtx_lock((struct mtx *)((c)->lock))
#define CHN_UNLOCK(c) mtx_unlock((struct mtx *)((c)->lock))
#define CHN_LOCKASSERT(c)

int fmtvalid(u_int32_t fmt, u_int32_t *fmtlist);

#define PCMDIR_PLAY 1
#define PCMDIR_REC -1

#define PCMTRIG_START 1
#define PCMTRIG_EMLDMAWR 2
#define PCMTRIG_EMLDMARD 3
#define PCMTRIG_STOP 0
#define PCMTRIG_ABORT -1

#define CHN_F_CLOSING           0x00000004  /* a pending close */
#define CHN_F_ABORTING          0x00000008  /* a pending abort */
#define CHN_F_RUNNING		0x00000010  /* dma is running */
#define CHN_F_TRIGGERED		0x00000020
#define CHN_F_NOTRIGGER		0x00000040

#define CHN_F_BUSY              0x00001000  /* has been opened 	*/
#define	CHN_F_HAS_SIZE		0x00002000  /* user set block size */
#define CHN_F_NBIO              0x00004000  /* do non-blocking i/o */
#define CHN_F_MAPPED		0x00010000  /* has been mmap()ed */
#define CHN_F_DEAD		0x00020000
#define CHN_F_BADSETTING	0x00040000

#define CHN_F_RESET		(CHN_F_BUSY | CHN_F_DEAD)

/*
 * This should be large enough to hold all pcm data between
 * tsleeps in chn_{read,write} at the highest sample rate.
 * (which is usually 48kHz * 16bit * stereo = 192000 bytes/sec)
 */
#define CHN_2NDBUFBLKSIZE	(2 * 1024)
/* The total number of blocks per secondary bufhard. */
#define CHN_2NDBUFBLKNUM	(32)
/* The size of a whole secondary bufhard. */
#define CHN_2NDBUFMAXSIZE	(131072)

#define	CHN_DEFAULT_HZ		50

#define CHANNEL_DECLARE(name) static DEFINE_CLASS(name, name ## _methods, sizeof(struct kobj))
