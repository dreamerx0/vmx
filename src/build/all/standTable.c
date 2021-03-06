/******************************************************************************
 *   DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS HEADER.
 *
 *   This file is part of Real VMX.
 *   Copyright (C) 2013 Surplus Users Ham Society
 *
 *   Real VMX is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   Real VMX is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with Real VMX.  If not, see <http://www.gnu.org/licenses/>.
 */

/* standTable.c - Standalone symbol table */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <a.out.h>
#include <time.h>
#include <setjmp.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/uio.h>
#include <private/stdioP.h>
#include <vmx.h>
#include <arch/vmxArchLib.h>
#include <arch/sigArchLib.h>
#include <arch/elfArchLib.h>
#include <os/objLib.h>
#include <os/classLib.h>
#include <os/classShow.h>
#include <os/errnoLib.h>
#include <os/memPartLib.h>
#include <os/memLib.h>
#include <os/memShow.h>
#include <os/taskHookLib.h>
#include <os/taskHookShow.h>
#include <os/iosLib.h>
#include <os/iosShow.h>
#include <os/logLib.h>
#include <os/ffsLib.h>
#include <os/symbol.h>
#include <os/symLib.h>
#include <os/envShow.h>
#include <os/erfLib.h>
#include <os/vmLib.h>
#include <os/vmShow.h>
#include <ostool/moduleLib.h>
#include <ostool/moduleShow.h>
#include <util/listLib.h>
#include <util/dllLib.h>
#include <util/sllLib.h>
#include <util/qLib.h>
#include <util/hashLib.h>
#include <util/rngLib.h>
#include <util/cksumLib.h>
#include <vmx/taskLib.h>
#include <vmx/taskInfo.h>
#include <vmx/taskShow.h>
#include <vmx/semLib.h>
#include <vmx/semShow.h>
#include <vmx/msgQLib.h>
#include <vmx/msgQShow.h>
#include <vmx/kernelLib.h>
#include <vmx/workQLib.h>
#include <vmx/tickLib.h>
#include <vmx/wdLib.h>
#include <vmx/wdShow.h>
#include <fs/xbdRamDisk.h>
#include <fs/xbdBlkDev.h>
#include <usr/usrLib.h>
#include <usr/usrFsLib.h>
#include <net/mbuf.h>
#include <net/netLib.h>
#include <net/netShow.h>
#include "ugl.h"

/* Gloabals */
int standTableSize = 0;
SYMBOL standTable[] =
{
  /* usrLib */
  {NULL, "_show", show, 0, N_TEXT | N_EXT},
  {NULL, "_taskIdFigure", taskIdFigure, 0, N_TEXT | N_EXT},
  {NULL, "_help", help, 0, N_TEXT | N_EXT},
  {NULL, "_i", i, 0, N_TEXT | N_EXT},
  {NULL, "_ti", ti, 0, N_TEXT | N_EXT},
  {NULL, "_sp", sp, 0, N_TEXT | N_EXT},
  {NULL, "_ts", ts, 0, N_TEXT | N_EXT},
  {NULL, "_tr", tr, 0, N_TEXT | N_EXT},
  {NULL, "_td", td, 0, N_TEXT | N_EXT},
  {NULL, "_d", d, 0, N_TEXT | N_EXT},
  {NULL, "_m", m, 0, N_TEXT | N_EXT},
  {NULL, "_pc", pc, 0, N_TEXT | N_EXT},
  {NULL, "_devs", devs, 0, N_TEXT | N_EXT},
  {NULL, "_ld", ld, 0, N_TEXT | N_EXT},
  {NULL, "_lkup", lkup, 0, N_TEXT | N_EXT},
  {NULL, "_checkStack", checkStack, 0, N_TEXT | N_EXT},
  {NULL, "_periodRun", periodRun, 0, N_TEXT | N_EXT},
  {NULL, "_period", period, 0, N_TEXT | N_EXT},
  {NULL, "_repeatRun", repeatRun, 0, N_TEXT | N_EXT},
  {NULL, "_repeat", repeat, 0, N_TEXT | N_EXT},
  {NULL, "_version", version, 0, N_TEXT | N_EXT},
  {NULL, "_printLogo", printLogo, 0, N_TEXT | N_EXT},

  /* usrFsLib */
  {NULL, "_ioHelp", ioHelp, 0, N_TEXT | N_EXT},
  {NULL, "_dirList", dirList, 0, N_TEXT | N_EXT},
  {NULL, "_cd", cd, 0, N_TEXT | N_EXT},
  {NULL, "_pwd", pwd, 0, N_TEXT | N_EXT},
  {NULL, "_ls", ls, 0, N_TEXT | N_EXT},
  {NULL, "_ll", ll, 0, N_TEXT | N_EXT},
  {NULL, "_lsr", lsr, 0, N_TEXT | N_EXT},
  {NULL, "_llr", llr, 0, N_TEXT | N_EXT},
  {NULL, "_copy", copy, 0, N_TEXT | N_EXT},

  /* stdio */
  {NULL, "_fpClassId", NULL, 0, N_DATA | N_EXT},
  {NULL, "_stdioFpCreate", stdioFpCreate, 0, N_TEXT | N_EXT},
  {NULL, "_stdioFpInit", stdioFpInit, 0, N_TEXT | N_EXT},
  {NULL, "_stdioFpDestroy", stdioFpDestroy, 0, N_TEXT | N_EXT},
  {NULL, "_ferror", ferror, 0, N_TEXT | N_EXT},
  {NULL, "_feof", feof, 0, N_TEXT | N_EXT},
  {NULL, "_fflush", fflush, 0, N_TEXT | N_EXT},
  {NULL, "_fopen", fopen, 0, N_TEXT | N_EXT},
  {NULL, "_fdopen", fdopen, 0, N_TEXT | N_EXT},
  {NULL, "_fclose", fclose, 0, N_TEXT | N_EXT},
  {NULL, "_setvbuf", setvbuf, 0, N_TEXT | N_EXT},
  {NULL, "_fread", fread, 0, N_TEXT | N_EXT},
  {NULL, "_fwrite", fwrite, 0, N_TEXT | N_EXT},
  {NULL, "_putc", putc, 0, N_TEXT | N_EXT},
  {NULL, "_fputc", fputc, 0, N_TEXT | N_EXT},
  {NULL, "_putchar", putchar, 0, N_TEXT | N_EXT},
  {NULL, "_ungetc", ungetc, 0, N_TEXT | N_EXT},
  {NULL, "_getc", getc, 0, N_TEXT | N_EXT},
  {NULL, "_fgetc", fgetc, 0, N_TEXT | N_EXT},
  {NULL, "_gets", gets, 0, N_TEXT | N_EXT},
  {NULL, "_fgets", fgets, 0, N_TEXT | N_EXT},
  {NULL, "_getchar", getchar, 0, N_TEXT | N_EXT},
  {NULL, "_puts", puts, 0, N_TEXT | N_EXT},
  {NULL, "_printf", printf, 0, N_TEXT | N_EXT},
  {NULL, "_asprintf", asprintf, 0, N_TEXT | N_EXT},
  {NULL, "_sprintf", sprintf, 0, N_TEXT | N_EXT},
  {NULL, "_snprintf", snprintf, 0, N_TEXT | N_EXT},
  {NULL, "_fprintf", fprintf, 0, N_TEXT | N_EXT},
  {NULL, "_fdprintf", fdprintf, 0, N_TEXT | N_EXT},
  {NULL, "_vdprintf", vdprintf, 0, N_TEXT | N_EXT},
  {NULL, "_vfnprintf", vfnprintf, 0, N_TEXT | N_EXT},
  {NULL, "_scanf", scanf, 0, N_TEXT | N_EXT},
  {NULL, "_sscanf", sscanf, 0, N_TEXT | N_EXT},
  {NULL, "_fscanf", fscanf, 0, N_TEXT | N_EXT},
  {NULL, "_vfscanf", vfscanf, 0, N_TEXT | N_EXT},

  /* stdlib */
  {NULL, "_malloc", malloc, 0, N_TEXT | N_EXT},
  {NULL, "_realloc", realloc, 0, N_TEXT | N_EXT},
  {NULL, "_memalign", memalign, 0, N_TEXT | N_EXT},
  {NULL, "_valloc", valloc, 0, N_TEXT | N_EXT},
  {NULL, "_free", free, 0, N_TEXT | N_EXT},
  {NULL, "_exit", exit, 0, N_TEXT | N_EXT},
  {NULL, "_abs", abs, 0, N_TEXT | N_EXT},
  {NULL, "_itoa", itoa, 0, N_TEXT | N_EXT},
  {NULL, "_itox", itox, 0, N_TEXT | N_EXT},
  {NULL, "_rand", rand, 0, N_TEXT | N_EXT},

  {NULL, "_getenv", getenv, 0, N_TEXT | N_EXT},
  {NULL, "_putenv", putenv, 0, N_TEXT | N_EXT},
  {NULL, "_envShow", envShow, 0, N_TEXT | N_EXT},

  /* string */
  {NULL, "_memchr", memchr, 0, N_TEXT | N_EXT},
  {NULL, "_memcmp", memcmp, 0, N_TEXT | N_EXT},
  {NULL, "_memcpy", memcpy, 0, N_TEXT | N_EXT},
  {NULL, "_memmove", memmove, 0, N_TEXT | N_EXT},
  {NULL, "_memset", memset, 0, N_TEXT | N_EXT},
  {NULL, "_strlen", strlen, 0, N_TEXT | N_EXT},
  {NULL, "_strcpy", strcpy, 0, N_TEXT | N_EXT},
  {NULL, "_strcat", strcat, 0, N_TEXT | N_EXT},
  {NULL, "_strchr", strchr, 0, N_TEXT | N_EXT},
  {NULL, "_strcmp", strcmp, 0, N_TEXT | N_EXT},

  /* ctype */
  {NULL, "_isalnum", isalnum, 0, N_TEXT | N_EXT},
  {NULL, "_isalpha", isalpha, 0, N_TEXT | N_EXT},
  {NULL, "_iscntrl", iscntrl, 0, N_TEXT | N_EXT},
  {NULL, "_isdigit", isdigit, 0, N_TEXT | N_EXT},
  {NULL, "_isgraph", isgraph, 0, N_TEXT | N_EXT},
  {NULL, "_islower", islower, 0, N_TEXT | N_EXT},
  {NULL, "_isprint", isprint, 0, N_TEXT | N_EXT},
  {NULL, "_ispunct", ispunct, 0, N_TEXT | N_EXT},
  {NULL, "_isspace", isspace, 0, N_TEXT | N_EXT},
  {NULL, "_isupper", isupper, 0, N_TEXT | N_EXT},
  {NULL, "_isxdigit", isxdigit, 0, N_TEXT | N_EXT},
  {NULL, "_tolower", tolower, 0, N_TEXT | N_EXT},
  {NULL, "_toupper", toupper, 0, N_TEXT | N_EXT},

  /* time */
  {NULL, "_time", time, 0, N_TEXT | N_EXT},
  {NULL, "_mktime", mktime, 0, N_TEXT | N_EXT},
  {NULL, "_localtime", localtime, 0, N_TEXT | N_EXT},
  {NULL, "_localtime_r", localtime_r, 0, N_TEXT | N_EXT},
  {NULL, "_strftime", strftime, 0, N_TEXT | N_EXT},

  /* setjmp */
  {NULL, "_longjmp", longjmp, 0, N_TEXT | N_EXT},
  {NULL, "_sigCtxSave", _sigCtxSave, 0, N_TEXT | N_EXT},
  {NULL, "_sigCtxLoad", _sigCtxLoad, 0, N_TEXT | N_EXT},
  {NULL, "_sigCtxStackEnd", _sigCtxStackEnd, 0, N_TEXT | N_EXT},
  {NULL, "_sigCtxSetup", _sigCtxSetup, 0, N_TEXT | N_EXT},

  /* fcntl */
  {NULL, "_open", open, 0, N_TEXT | N_EXT},
  {NULL, "_creat", creat, 0, N_TEXT | N_EXT},

  /* unistd */
  {NULL, "_lseek", lseek, 0, N_TEXT | N_EXT},
  {NULL, "_read", read, 0, N_TEXT | N_EXT},
  {NULL, "_write", write, 0, N_TEXT | N_EXT},
  {NULL, "_getcwd", getcwd, 0, N_TEXT | N_EXT},
  {NULL, "_chdir", chdir, 0, N_TEXT | N_EXT},
  {NULL, "_rmdir", rmdir, 0, N_TEXT | N_EXT},
  {NULL, "_isatty", isatty, 0, N_TEXT | N_EXT},
  {NULL, "_close", close, 0, N_TEXT | N_EXT},
  {NULL, "_fpathconf", fpathconf, 0, N_TEXT | N_EXT},
  {NULL, "_pathconf", pathconf, 0, N_TEXT | N_EXT},
  {NULL, "_ftruncate", ftruncate, 0, N_TEXT | N_EXT},
  {NULL, "_symlink", symlink, 0, N_TEXT | N_EXT},
  {NULL, "_readlink", readlink, 0, N_TEXT | N_EXT},

  /* dirent */
  {NULL, "_closedir", closedir, 0, N_TEXT | N_EXT},
  {NULL, "_opendir", opendir, 0, N_TEXT | N_EXT},
  {NULL, "_readdir", readdir, 0, N_TEXT | N_EXT},
  {NULL, "_rewinddir", rewinddir, 0, N_TEXT | N_EXT},

  /* ioctl */
  {NULL, "_ioctl", ioctl, 0, N_TEXT | N_EXT},

  /* uio */
  {NULL, "_uiomove", uiomove, 0, N_TEXT | N_EXT},

  /* stat */
  {NULL, "_fstat", fstat, 0, N_TEXT | N_EXT},
  {NULL, "_lstat", lstat, 0, N_TEXT | N_EXT},
  {NULL, "_mkdir", mkdir, 0, N_TEXT | N_EXT},
  {NULL, "_stat", stat, 0, N_TEXT | N_EXT},

  /* iosLib */
  {NULL, "_iosDevShow", iosDevShow, 0, N_TEXT | N_EXT},
  {NULL, "_iosFdShow", iosFdShow, 0, N_TEXT | N_EXT},

  /* logLib */
  {NULL, "_logMsg", logMsg, 0, N_TEXT | N_EXT},

  /* erfLib */
  {NULL, "_erfHandlerRegister", erfHandlerRegister, 0, N_TEXT | N_EXT},
  {NULL, "_erfHandlerUnregister", erfHandlerUnregister, 0, N_TEXT | N_EXT},
  {NULL, "_erfEventRaise", erfEventRaise, 0, N_TEXT | N_EXT},
  {NULL, "_erfCategoryAllocate", erfCategoryAllocate, 0, N_TEXT | N_EXT},
  {NULL, "_erfTypeAllocate", erfTypeAllocate, 0, N_TEXT | N_EXT},

  /* classLib */
  {NULL, "_rootClassId", NULL, 0, N_DATA | N_EXT},
  {NULL, "_classCreate", classCreate, 0, N_TEXT | N_EXT},
  {NULL, "_classInit", classInit, 0, N_TEXT | N_EXT},
  {NULL, "_classShowConnect", classShowConnect, 0, N_TEXT | N_EXT},
  {NULL, "_classDestroy", classDestroy, 0, N_TEXT | N_EXT},
  {NULL, "_classShow", classShow, 0, N_TEXT | N_EXT},

  /* objLib */
  {NULL, "_objAllocPad", objAllocPad, 0, N_TEXT | N_EXT},
  {NULL, "_objAlloc", objAlloc, 0, N_TEXT | N_EXT},
  {NULL, "_objShow", objShow, 0, N_TEXT | N_EXT},
  {NULL, "_objFree", objFree, 0, N_TEXT | N_EXT},
  {NULL, "_objCoreInit", objCoreInit, 0, N_TEXT | N_EXT},
  {NULL, "_objCoreTerminate", objCoreTerminate, 0, N_TEXT | N_EXT},

  /* listLib */
  {NULL, "_listInit", listInit, 0, N_TEXT | N_EXT},
  {NULL, "_listInsert", listInsert, 0, N_TEXT | N_EXT},
  {NULL, "_listAdd", listAdd, 0, N_TEXT | N_EXT},
  {NULL, "_listGet", listGet, 0, N_TEXT | N_EXT},
  {NULL, "_listRemove", listRemove, 0, N_TEXT | N_EXT},
  {NULL, "_listCount", listCount, 0, N_TEXT | N_EXT},

  /* dllLib */
  {NULL, "_dllInit", dllInit, 0, N_TEXT | N_EXT},
  {NULL, "_dllAdd", dllAdd, 0, N_TEXT | N_EXT},
  {NULL, "_dllRemove", dllRemove, 0, N_TEXT | N_EXT},
  {NULL, "_dllInsert", dllInsert, 0, N_TEXT | N_EXT},
  {NULL, "_dllGet", dllGet, 0, N_TEXT | N_EXT},
  {NULL, "_dllCount", dllCount, 0, N_TEXT | N_EXT},

  /* sllLib */
  {NULL, "_sllInit", sllInit, 0, N_TEXT | N_EXT},
  {NULL, "_sllPutAtHead", sllPutAtHead, 0, N_TEXT | N_EXT},
  {NULL, "_sllPutAtTail", sllPutAtTail, 0, N_TEXT | N_EXT},
  {NULL, "_sllPrevious", sllPrevious, 0, N_TEXT | N_EXT},
  {NULL, "_sllAdd", sllAdd, 0, N_TEXT | N_EXT},
  {NULL, "_sllRemove", sllRemove, 0, N_TEXT | N_EXT},
  {NULL, "_sllInsert", sllInsert, 0, N_TEXT | N_EXT},
  {NULL, "_sllGet", sllGet, 0, N_TEXT | N_EXT},
  {NULL, "_sllCount", sllCount, 0, N_TEXT | N_EXT},
  {NULL, "_sllEach", sllEach, 0, N_TEXT | N_EXT},

  /* qLib */
  {NULL, "_qCreate", qCreate, 0, N_TEXT | N_EXT},
  {NULL, "_qInit", qInit, 0, N_TEXT | N_EXT},
  {NULL, "_qDestroy", qDestroy, 0, N_TEXT | N_EXT},
  {NULL, "_qTerminate", qTerminate, 0, N_TEXT | N_EXT},
  {NULL, "_qFirst", qFirst, 0, N_TEXT | N_EXT},
  {NULL, "_qPut", qPut, 0, N_TEXT | N_EXT},
  {NULL, "_qGet", qGet, 0, N_TEXT | N_EXT},
  {NULL, "_qRemove", qRemove, 0, N_TEXT | N_EXT},
  {NULL, "_qMove", qMove, 0, N_TEXT | N_EXT},
  {NULL, "_qAdvance", qAdvance, 0, N_TEXT | N_EXT},
  {NULL, "_qExpired", qExpired, 0, N_TEXT | N_EXT},
  {NULL, "_qKey", qKey, 0, N_TEXT | N_EXT},
  {NULL, "_qOffset", qOffset, 0, N_TEXT | N_EXT},
  {NULL, "_qInfo", qInfo, 0, N_TEXT | N_EXT},
  {NULL, "_qEach", qEach, 0, N_TEXT | N_EXT},

  /* hashLib */
  {NULL, "_hashTableCreate", hashTableCreate, 0, N_TEXT | N_EXT},
  {NULL, "_hashTableInit", hashTableInit, 0, N_TEXT | N_EXT},
  {NULL, "_hashTableDestroy", hashTableDestroy, 0, N_TEXT | N_EXT},
  {NULL, "_hashTableTerminate", hashTableTerminate, 0, N_TEXT | N_EXT},
  {NULL, "_hashTableDelete", hashTableDelete, 0, N_TEXT | N_EXT},
  {NULL, "_hashTablePut", hashTablePut, 0, N_TEXT | N_EXT},
  {NULL, "_hashTableFind", hashTableFind, 0, N_TEXT | N_EXT},
  {NULL, "_hashTableEach", hashTableEach, 0, N_TEXT | N_EXT},
  {NULL, "_hashTableRemove", hashTableRemove, 0, N_TEXT | N_EXT},
  {NULL, "_hashKeyCmp", hashKeyCmp, 0, N_TEXT | N_EXT},
  {NULL, "_hashStringCmp", hashStringCmp, 0, N_TEXT | N_EXT},

  /* rngLib */
  {NULL, "_rngCreate", rngCreate, 0, N_TEXT | N_EXT},
  {NULL, "_rngDelete", rngDelete, 0, N_TEXT | N_EXT},
  {NULL, "_rngFlush", rngFlush, 0, N_TEXT | N_EXT},
  {NULL, "_rngBufGet", rngBufGet, 0, N_TEXT | N_EXT},
  {NULL, "_rngBufPut", rngBufPut, 0, N_TEXT | N_EXT},
  {NULL, "_rngIsEmpty", rngIsEmpty, 0, N_TEXT | N_EXT},
  {NULL, "_rngIsFull", rngIsFull, 0, N_TEXT | N_EXT},
  {NULL, "_rngFreeBytes", rngFreeBytes, 0, N_TEXT | N_EXT},
  {NULL, "_rngNBytes", rngNBytes, 0, N_TEXT | N_EXT},
  {NULL, "_rngPutAhead", rngPutAhead, 0, N_TEXT | N_EXT},
  {NULL, "_rngMoveAhead", rngMoveAhead, 0, N_TEXT | N_EXT},
  {NULL, "_rngElemGet", rngElemGet, 0, N_TEXT | N_EXT},
  {NULL, "_rngElemPut", rngElemPut, 0, N_TEXT | N_EXT},

  /* cksumLib */
  {NULL, "_checksum", checksum, 0, N_TEXT | N_EXT},

  /* errnoLib */
  {NULL, "_errnoSet", errnoSet, 0, N_TEXT | N_EXT},
  {NULL, "_errnoGet", errnoGet, 0, N_TEXT | N_EXT},

  /* taskLib */
  {NULL, "_taskClassId", NULL, 0, N_DATA | N_EXT},
  {NULL, "_taskSpawn", taskSpawn, 0, N_TEXT | N_EXT},
  {NULL, "_taskCreat", taskCreat, 0, N_TEXT | N_EXT},
  {NULL, "_taskInit", taskInit, 0, N_TEXT | N_EXT},
  {NULL, "_taskDestroy", taskDestroy, 0, N_TEXT | N_EXT},
  {NULL, "_taskDelete", taskDelete, 0, N_TEXT | N_EXT},
  {NULL, "_taskDeleteForce", taskDeleteForce, 0, N_TEXT | N_EXT},
  {NULL, "_taskActivate", taskActivate, 0, N_TEXT | N_EXT},
  {NULL, "_taskSuspend", taskSuspend, 0, N_TEXT | N_EXT},
  {NULL, "_taskResume", taskResume, 0, N_TEXT | N_EXT},
  {NULL, "_taskDelay", taskDelay, 0, N_TEXT | N_EXT},
  {NULL, "_taskUndelay", taskUndelay, 0, N_TEXT | N_EXT},
  {NULL, "_taskPrioritySet", taskPrioritySet, 0, N_TEXT | N_EXT},
  {NULL, "_taskPriorityGet", taskPriorityGet, 0, N_TEXT | N_EXT},
  {NULL, "_taskRestart", taskRestart, 0, N_TEXT | N_EXT},
  {NULL, "_taskExit", taskExit, 0, N_TEXT | N_EXT},
  {NULL, "_taskLock", taskLock, 0, N_TEXT | N_EXT},
  {NULL, "_taskUnlock", taskUnlock, 0, N_TEXT | N_EXT},
  {NULL, "_taskSafe", taskSafe, 0, N_TEXT | N_EXT},
  {NULL, "_taskUnsafe", taskUnsafe, 0, N_TEXT | N_EXT},
  {NULL, "_taskIdSelf", taskIdSelf, 0, N_TEXT | N_EXT},
  {NULL, "_taskTcb", taskTcb, 0, N_TEXT | N_EXT},
  {NULL, "_taskStackAllot", taskStackAllot, 0, N_TEXT | N_EXT},
  {NULL, "_taskIdVerify", taskIdVerify, 0, N_TEXT | N_EXT},
  {NULL, "_taskIdDefault", taskIdDefault, 0, N_TEXT | N_EXT},
  {NULL, "_taskName", taskName, 0, N_TEXT | N_EXT},
  {NULL, "_taskRegsGet", taskRegsGet, 0, N_TEXT | N_EXT},
  {NULL, "_taskIdListGet", taskIdListGet, 0, N_TEXT | N_EXT},
  {NULL, "_taskOptionsSet", taskOptionsSet, 0, N_TEXT | N_EXT},
  {NULL, "_taskShow", taskShow, 0, N_TEXT | N_EXT},

  /* memLib */
  {NULL, "_memPartClassId", NULL, 0, N_DATA | N_EXT},
  {NULL, "_memSysPartId", NULL, 0, N_DATA | N_EXT},
  {NULL, "_memPartCreate", memPartCreate, 0, N_TEXT | N_EXT},
  {NULL, "_memPartInit", memPartInit, 0, N_TEXT | N_EXT},
  {NULL, "_memPartDestroy", memPartDestroy, 0, N_TEXT | N_EXT},
  {NULL, "_memPartAddToPool", memPartAddToPool, 0, N_TEXT | N_EXT},
  {NULL, "_memPartAlignedAlloc", memPartAlignedAlloc, 0, N_TEXT | N_EXT},
  {NULL, "_memPartAlloc", memPartAlloc, 0, N_TEXT | N_EXT},
  {NULL, "_memPartRealloc", memPartRealloc, 0, N_TEXT | N_EXT},
  {NULL, "_memPartFree", memPartFree, 0, N_TEXT | N_EXT},
  {NULL, "_memPartOptionsSet", memPartOptionsSet, 0, N_TEXT | N_EXT},
  {NULL, "_memPartShow", memPartShow, 0, N_TEXT | N_EXT},

  /* semLib */
  {NULL, "_semClassId", NULL, 0, N_DATA | N_EXT},
  {NULL, "_semCreate", semCreate, 0, N_TEXT | N_EXT},
  {NULL, "_semInit", semInit, 0, N_TEXT | N_EXT},
  {NULL, "_semGive", semGive, 0, N_TEXT | N_EXT},
  {NULL, "_semTake", semTake, 0, N_TEXT | N_EXT},
  {NULL, "_semDelete", semDelete, 0, N_TEXT | N_EXT},
  {NULL, "_semDestroy", semDestroy, 0, N_TEXT | N_EXT},
  {NULL, "_semInvalid", semInvalid, 0, N_TEXT | N_EXT},
  {NULL, "_semQFlush", semQFlush, 0, N_TEXT | N_EXT},
  {NULL, "_semQFlushDefer", semQFlushDefer, 0, N_TEXT | N_EXT},
  {NULL, "_semShow", semShow, 0, N_TEXT | N_EXT},
  {NULL, "_semBCreate", semBCreate, 0, N_TEXT | N_EXT},
  {NULL, "_semBInit", semBInit, 0, N_TEXT | N_EXT},
  {NULL, "_semMCreate", semMCreate, 0, N_TEXT | N_EXT},
  {NULL, "_semMInit", semMInit, 0, N_TEXT | N_EXT},
  {NULL, "_semCCreate", semCCreate, 0, N_TEXT | N_EXT},
  {NULL, "_semCInit", semCInit, 0, N_TEXT | N_EXT},

  /* msgQLib */
  {NULL, "_msgQClassId", NULL, 0, N_DATA | N_EXT},
  {NULL, "_msgQCreate", msgQCreate, 0, N_TEXT | N_EXT},
  {NULL, "_msgQInit", msgQInit, 0, N_TEXT | N_EXT},
  {NULL, "_msgQTerminate", msgQTerminate, 0, N_TEXT | N_EXT},
  {NULL, "_msgQDelete", msgQDelete, 0, N_TEXT | N_EXT},
  {NULL, "_msgQDestroy", msgQDestroy, 0, N_TEXT | N_EXT},
  {NULL, "_msgQSend", msgQSend, 0, N_TEXT | N_EXT},
  {NULL, "_msgQReceive", msgQReceive, 0, N_TEXT | N_EXT},
  {NULL, "_msgQInfoGet", msgQInfoGet, 0, N_TEXT | N_EXT},
  {NULL, "_msgQShow", msgQShow, 0, N_TEXT | N_EXT},

  /* ffsLib */
  {NULL, "_ffsLsb", ffsLsb, 0, N_TEXT | N_EXT},
  {NULL, "_ffsMsb", ffsMsb, 0, N_TEXT | N_EXT},

  /* kernelLib */
  {NULL, "_kernelTimeSlice", kernelTimeSlice, 0, N_TEXT | N_EXT},

  /* workQLib */
  {NULL, "_workQAdd0", workQAdd0, 0, N_TEXT | N_EXT},
  {NULL, "_workQAdd1", workQAdd1, 0, N_TEXT | N_EXT},
  {NULL, "_workQAdd2", workQAdd2, 0, N_TEXT | N_EXT},

  /* tickLib */
  {NULL, "_tickSet", tickSet, 0, N_TEXT | N_EXT},
  {NULL, "_tickGet", tickGet, 0, N_TEXT | N_EXT},
  {NULL, "_tick64Set", tick64Set, 0, N_TEXT | N_EXT},
  {NULL, "_tick64Get", tick64Get, 0, N_TEXT | N_EXT},

  /* symLib */
  {NULL, "_symTableClassId", NULL, 0, N_DATA | N_EXT},
  {NULL, "_symTableCreate", symTableCreate, 0, N_TEXT | N_EXT},
  {NULL, "_symTableInit", symTableInit, 0, N_TEXT | N_EXT},
  {NULL, "_symTableDestroy", symTableDestroy, 0, N_TEXT | N_EXT},
  {NULL, "_symTableDelete", symTableDelete, 0, N_TEXT | N_EXT},
  {NULL, "_symTableTerminate", symTableTerminate, 0, N_TEXT | N_EXT},
  {NULL, "_symCreate", symCreate, 0, N_TEXT | N_EXT},
  {NULL, "_symInit", symInit, 0, N_TEXT | N_EXT},
  {NULL, "_symDestroy", symDestroy, 0, N_TEXT | N_EXT},
  {NULL, "_symAdd", symAdd, 0, N_TEXT | N_EXT},
  {NULL, "_symTableAdd", symTableAdd, 0, N_TEXT | N_EXT},
  {NULL, "_symFindByName", symFindByName, 0, N_TEXT | N_EXT},
  {NULL, "_symFindByNameAndType", symFindByNameAndType, 0, N_TEXT | N_EXT},
  {NULL, "_symFindSymbol", symFindSymbol, 0, N_TEXT | N_EXT},
  {NULL, "_symNameGet", symNameGet, 0, N_TEXT | N_EXT},
  {NULL, "_symValueGet", symValueGet, 0, N_TEXT | N_EXT},
  {NULL, "_symTypeGet", symTypeGet, 0, N_TEXT | N_EXT},
  {NULL, "_symEach", symEach, 0, N_TEXT | N_EXT},
  {NULL, "_symRemove", symRemove, 0, N_TEXT | N_EXT},
  {NULL, "_symTableRemove", symTableRemove, 0, N_TEXT | N_EXT},

  /* taskHookLib */
  {NULL, "_taskCreateHookAdd", taskCreateHookAdd, 0, N_TEXT | N_EXT},
  {NULL, "_taskCreateHookDelete", taskCreateHookDelete, 0, N_TEXT | N_EXT},
  {NULL, "_taskCreateHookShow", taskCreateHookShow, 0, N_TEXT | N_EXT},
  {NULL, "_taskSwitchHookAdd", taskSwitchHookAdd, 0, N_TEXT | N_EXT},
  {NULL, "_taskSwitchHookDelete", taskSwitchHookDelete, 0, N_TEXT | N_EXT},
  {NULL, "_taskSwitchHookShow", taskSwitchHookShow, 0, N_TEXT | N_EXT},
  {NULL, "_taskDeleteHookAdd", taskDeleteHookAdd, 0, N_TEXT | N_EXT},
  {NULL, "_taskDeleteHookDelete", taskDeleteHookDelete, 0, N_TEXT | N_EXT},
  {NULL, "_taskDeleteHookShow", taskDeleteHookShow, 0, N_TEXT | N_EXT},
  {NULL, "_taskSwapHookAdd", taskSwapHookAdd, 0, N_TEXT | N_EXT},
  {NULL, "_taskSwapHookDelete", taskSwapHookDelete, 0, N_TEXT | N_EXT},
  {NULL, "_taskSwapHookAttach", taskSwapHookAttach, 0, N_TEXT | N_EXT},
  {NULL, "_taskSwapHookDetach", taskSwapHookDetach, 0, N_TEXT | N_EXT},
  {NULL, "_taskSwapHookShow", taskSwapHookShow, 0, N_TEXT | N_EXT},

  /* wdLib */
  {NULL, "_wdCreate", wdCreate, 0, N_TEXT | N_EXT},
  {NULL, "_wdInit", wdInit, 0, N_TEXT | N_EXT},
  {NULL, "_wdDestroy", wdDestroy, 0, N_TEXT | N_EXT},
  {NULL, "_wdDelete", wdDelete, 0, N_TEXT | N_EXT},
  {NULL, "_wdTerminate", wdTerminate, 0, N_TEXT | N_EXT},
  {NULL, "_wdStart", wdStart, 0, N_TEXT | N_EXT},
  {NULL, "_wdCancel", wdCancel, 0, N_TEXT | N_EXT},
  {NULL, "_wdShow", wdShow, 0, N_TEXT | N_EXT},

  /* vmxArchLib */
  {NULL, "_intEnt", intEnt, 0, N_TEXT | N_EXT},
  {NULL, "_intExit", intExit, 0, N_TEXT | N_EXT},
  {NULL, "_vmxExit", vmxExit, 0, N_TEXT | N_EXT},
  {NULL, "_vmxTaskContextLoad", vmxTaskContextLoad, 0, N_TEXT | N_EXT},
  {NULL, "_vmxTaskEntry", vmxTaskEntry, 0, N_TEXT | N_EXT},
  {NULL, "_vmxTaskEntry", vmxTaskEntry, 0, N_TEXT | N_EXT},
  {NULL, "_intStackSet", intStackSet, 0, N_TEXT | N_EXT},
  {NULL, "_intStackEnable", intStackEnable, 0, N_TEXT | N_EXT},

  /* vmLib */
  {NULL, "_vmContextClassId", NULL, 0, N_DATA | N_EXT},
  {NULL, "_vmLibInit", vmLibInit, 0, N_TEXT | N_EXT},
  {NULL, "_vmGlobalMapInit", vmGlobalMapInit, 0, N_TEXT | N_EXT},
  {NULL, "_vmContextCreate", vmContextCreate, 0, N_TEXT | N_EXT},
  {NULL, "_vmContextInit", vmContextInit, 0, N_TEXT | N_EXT},
  {NULL, "_vmContextDestroy", vmContextDestroy, 0, N_TEXT | N_EXT},
  {NULL, "_vmStateSet", vmStateSet, 0, N_TEXT | N_EXT},
  {NULL, "_vmStateGet", vmStateGet, 0, N_TEXT | N_EXT},
  {NULL, "_vmMap", vmMap, 0, N_TEXT | N_EXT},
  {NULL, "_vmGlobalMap", vmGlobalMap, 0, N_TEXT | N_EXT},
  {NULL, "_vmGlobalInfoGet", vmGlobalInfoGet, 0, N_TEXT | N_EXT},
  {NULL, "_vmPageBlockSizeGet", vmPageBlockSizeGet, 0, N_TEXT | N_EXT},
  {NULL, "_vmCurrentSet", vmCurrentSet, 0, N_TEXT | N_EXT},
  {NULL, "_vmCurrentGet", vmCurrentGet, 0, N_TEXT | N_EXT},
  {NULL, "_vmEnable", vmEnable, 0, N_TEXT | N_EXT},
  {NULL, "_vmPageSizeGet", vmPageSizeGet, 0, N_TEXT | N_EXT},
  {NULL, "_vmTranslate", vmTranslate, 0, N_TEXT | N_EXT},
  {NULL, "_vmShowInit", vmShowInit, 0, N_TEXT | N_EXT},
  {NULL, "_vmContextShow", vmContextShow, 0, N_TEXT | N_EXT},

  /* moduleLib */
  {NULL, "_moduleClassId", NULL, 0, N_DATA | N_EXT},
  {NULL, "_moduleCreate", moduleCreate, 0, N_TEXT | N_EXT},
  {NULL, "_moduleInit", moduleInit, 0, N_TEXT | N_EXT},
  {NULL, "_moduleDestroy", moduleDestroy, 0, N_TEXT | N_EXT},
  {NULL, "_moduleTerminate", moduleTerminate, 0, N_TEXT | N_EXT},
  {NULL, "_moduleDelete", moduleDelete, 0, N_TEXT | N_EXT},
  {NULL, "_moduleIdFigure", moduleIdFigure, 0, N_TEXT | N_EXT},
  {NULL, "_moduleIdListGet", moduleIdListGet, 0, N_TEXT | N_EXT},
  {NULL, "_moduleNameGet", moduleNameGet, 0, N_TEXT | N_EXT},
  {NULL, "_moduleGroupGet", moduleGroupGet, 0, N_TEXT | N_EXT},
  {NULL, "_moduleFlagsGet", moduleFlagsGet, 0, N_TEXT | N_EXT},
  {NULL, "_moduleFormatGet", moduleFormatGet, 0, N_TEXT | N_EXT},
  {NULL, "_moduleFindByName", moduleFindByName, 0, N_TEXT | N_EXT},
  {NULL, "_moduleFindByNameAndPath", moduleFindByNameAndPath, 0, N_TEXT | N_EXT},
  {NULL, "_moduleFindByGroup", moduleFindByGroup, 0, N_TEXT | N_EXT},
  {NULL, "_moduleEach", moduleEach, 0, N_TEXT | N_EXT},
  {NULL, "_moduleInfoGet", moduleInfoGet, 0, N_TEXT | N_EXT},
  {NULL, "_moduleCheck", moduleCheck, 0, N_TEXT | N_EXT},
  {NULL, "_moduleCreateHookAdd", moduleCreateHookAdd, 0, N_TEXT | N_EXT},
  {NULL, "_moduleCreateHookDelete", moduleCreateHookDelete, 0, N_TEXT | N_EXT},
  {NULL, "_moduleSegAdd", moduleSegAdd, 0, N_TEXT | N_EXT},
  {NULL, "_moduleSegGet", moduleSegGet, 0, N_TEXT | N_EXT},
  {NULL, "_moduleSegFirst", moduleSegFirst, 0, N_TEXT | N_EXT},
  {NULL, "_moduleSegNext", moduleSegNext, 0, N_TEXT | N_EXT},
  {NULL, "_moduleSegEach", moduleSegEach, 0, N_TEXT | N_EXT},
  {NULL, "_moduleSegInfoGet", moduleSegInfoGet, 0, N_TEXT | N_EXT},
  {NULL, "_moduleShow", moduleShow, 0, N_TEXT | N_EXT},
  {NULL, "_elfArchLibInit", elfArchLibInit, 0, N_TEXT | N_EXT},

  /* netLib */
  {NULL, "_netJobAdd", netJobAdd, 0, N_TEXT | N_EXT},
  {NULL, "_netPoolShow", netPoolShow, 0, N_TEXT | N_EXT},
  {NULL, "_ifShow", ifShow, 0, N_TEXT | N_EXT},
  {NULL, "_routeShow", routeShow, 0, N_TEXT | N_EXT},
  {NULL, "_domainShow", domainShow, 0, N_TEXT | N_EXT},

  /* ugl */
  {NULL, "_uglOSMemPoolCreate", uglOSMemPoolCreate, 0, N_TEXT | N_EXT},
  {NULL, "_uglOSMemPoolDestroy", uglOSMemPoolDestroy, 0, N_TEXT | N_EXT},
  {NULL, "_uglOSMemAlloc", uglOSMemAlloc, 0, N_TEXT | N_EXT},
  {NULL, "_uglOSMemCalloc", uglOSMemCalloc, 0, N_TEXT | N_EXT},
  {NULL, "_uglOSMemRealloc", uglOSMemRealloc, 0, N_TEXT | N_EXT},
  {NULL, "_uglOSMemFree", uglOSMemFree, 0, N_TEXT | N_EXT},
  {NULL, "_uglOSMemSizeGet", uglOSMemSizeGet, 0, N_TEXT | N_EXT},
  {NULL, "_uglMemDefaultPoolGet", uglMemDefaultPoolGet, 0, N_TEXT | N_EXT},
  {NULL, "_uglMemDefaultPoolSet", uglMemDefaultPoolSet, 0, N_TEXT | N_EXT},
  {NULL, "_uglRegionCreate", uglRegionCreate, 0, N_TEXT | N_EXT},
  {NULL, "_uglRegionInit", uglRegionInit, 0, N_TEXT | N_EXT},
  {NULL, "_uglRegionDeinit", uglRegionDeinit, 0, N_TEXT | N_EXT},
  {NULL, "_uglRegionDestroy", uglRegionDestroy, 0, N_TEXT | N_EXT},
  {NULL, "_uglRegionIsEmpty", uglRegionIsEmpty, 0, N_TEXT | N_EXT},
  {NULL, "_uglRegionEmpty", uglRegionEmpty, 0, N_TEXT | N_EXT},
  {NULL, "_uglRegionCopy", uglRegionCopy, 0, N_TEXT | N_EXT},
  {NULL, "_uglRegionMove", uglRegionMove, 0, N_TEXT | N_EXT},
  {NULL, "_uglRegionRectInclude", uglRegionRectInclude, 0, N_TEXT | N_EXT},
  {NULL, "_uglRegionRectExclude", uglRegionRectExclude, 0, N_TEXT | N_EXT},
  {NULL, "_uglRegionIntersect", uglRegionIntersect, 0, N_TEXT | N_EXT},
  {NULL, "_uglRegionRectSortedGet", uglRegionRectSortedGet, 0, N_TEXT | N_EXT},
  {NULL, "_uglRegionShow", uglRegionShow, 0, N_TEXT | N_EXT},
  {NULL, "_uglLine", uglLine, 0, N_TEXT | N_EXT},
  {NULL, "_uglRectangle", uglRectangle, 0, N_TEXT | N_EXT},
  {NULL, "_uglColorCubeCreate", uglColorCubeCreate, 0, N_TEXT | N_EXT},
  {NULL, "_uglColorCubeDestroy", uglColorCubeDestroy, 0, N_TEXT | N_EXT},
  {NULL, "_uglColorCubeAlloc", uglColorCubeAlloc, 0, N_TEXT | N_EXT},
  {NULL, "_uglColorCubeFree", uglColorCubeFree, 0, N_TEXT | N_EXT},
  {NULL, "_uglColorCubeLookupExt", uglColorCubeLookupExt, 0, N_TEXT | N_EXT},
  {NULL, "_uglColorCubeLookup", uglColorCubeLookup, 0, N_TEXT | N_EXT},

  /* xbd devices */
  {NULL, "_xbdRamDiskDevCreate", xbdRamDiskDevCreate, 0, N_TEXT | N_EXT},
  {NULL, "_xbdRamDiskDevDelete", xbdRamDiskDevDelete, 0, N_TEXT | N_EXT},
  {NULL, "_xbdBlkDevCreate", xbdBlkDevCreate, 0, N_TEXT | N_EXT},
  {NULL, "_xbdBlkDevDelete", xbdBlkDevDelete, 0, N_TEXT | N_EXT}
};

/* Locals */
LOCAL void standTableAddData(
    char *name,
    void *data
    );

/* Functions */

/******************************************************************************
 * standTableInit - Initialize standalone symbol table
 *
 * RETURNS: N/A
 */

void standTableInit(
    void
    )
{
    /* Get symbol table size */
    standTableSize = NELEMENTS(standTable);

    /* Add data symbols */
    standTableAddData("_fpClassId", fpClassId);
    standTableAddData("_rootClassId", rootClassId);
    standTableAddData("_taskClassId", taskClassId);
    standTableAddData("_memPartClassId", memPartClassId);
    standTableAddData("_memSysPartId", memSysPartId);
    standTableAddData("_semClassId", semClassId);
    standTableAddData("_msgQClassId", msgQClassId);
    standTableAddData("_hashClassId", hashClassId);
    standTableAddData("_vmContextClassId", vmContextClassId);
    standTableAddData("_symTableClassId", symTableClassId);
}

/******************************************************************************
 * standTableAddData - Add data symbol to symbol table
 *
 * RETURNS: N/A
 */

LOCAL void standTableAddData(
    char *name,
    void *data
    )
{
    int i;

    for (i = 0; i < standTableSize; i++)
    {
        if (strcmp(name, standTable[i].name) == 0)
        {
            standTable[i].value = data;
            break;
        }
    }
}

