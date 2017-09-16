(import 'sys/code.inc)

(when (eql *cpu* 'x86_64)
	(defcmacro sys-push-trashed ()
		'(vp-push r1 r2 r5 r6 r7 r8 r9 r10))
	(defcmacro sys-pop-trashed ()
		'(vp-pop r1 r2 r5 r6 r7 r8 r9 r10)))

(when (eql *cpu* 'ARM)
	(defcmacro sys-push-trashed ())
	(defcmacro sys-pop-trashed ()))

(when (eql *os* 'Darwin)

;;;;;;;;;;;;;;;;;
; syscall for OSX
;;;;;;;;;;;;;;;;;

(defcfun sys-call (_)
	(vp-cpy-cr (add _ 0x2000000) r0)
	(vp-syscall))

(defcvar 'sys_exit 0x1)
(defcvar 'sys_read 0x3)
(defcvar 'sys_write 0x4)
(defcvar 'sys_open 0x5)
(defcvar 'sys_close 0x6)
(defcvar 'sys_unlink 0xa)
(defcvar 'sys_ftruncate 0xc9)
(defcvar 'sys_stat 0xbc)
(defcvar 'sys_mmap 0xc5)
(defcvar 'sys_munmap 0x49)
(defcvar 'sys_mprotect 0x4a)
(defcvar 'sys_gettimeofday 0x74)
(defcvar 'sys_fcntl 0x5C)

(defcvar 'prot_none 0x0)
(defcvar 'prot_read 0x1)
(defcvar 'prot_write 0x2)
(defcvar 'prot_exec 0x4)

(defcvar 'map_shared 0x1)
(defcvar 'map_private 0x2)
(defcvar 'map_fixed 0x10)
(defcvar 'map_anon 0x1000)

(defcvar 'o_rdonly 0x0)
(defcvar 'o_wronly 0x1)
(defcvar 'o_rdwr 0x2)
(defcvar 'o_trunc 0x400)
(defcvar 'o_append 0x8)
(defcvar 'o_nonblock 0x4)
(defcvar 'o_creat 0x200)
(defcvar 'o_excl 0x800)
(defcvar 'o_nofollow 0x100)
(defcvar 'o_cloexec 0x1000000)

(defcvar 's_irwxu 0x1c0)
(defcvar 's_irusr 0x100)
(defcvar 's_iwusr 0x80)
(defcvar 's_ixusr 0x40)
(defcvar 's_irwxg 0x38)
(defcvar 's_irgrp 0x20)
(defcvar 's_iwgrp 0x10)
(defcvar 's_ixgrp 0x8)
(defcvar 's_irwxo 0x7)
(defcvar 's_iroth 0x4)
(defcvar 's_iwoth 0x2)
(defcvar 's_ixoth 0x1)
(defcvar 's_isuid 0x800)
(defcvar 's_isgid 0x400)
(defcvar 's_isvtx 0x200)

(defcvar 's_ifmt 0xf000)
(defcvar 's_ifdir 0x4000)
(defcvar 's_ifreg 0x8000)

(defcvar 'f_getfl 0x3)
(defcvar 'f_setfl 0x4)

(defcvar 'stat_mode 0x8)
(defcvar 'stat_fsize 0x48)
(defcvar 'stat_size 0x100)
(defcvar 'stat_mtime 0x38)

(defcfun sys-gettimeofday (_1)
	;time, timezone
	(assign _1 '(r6 r5))
	(vp-push r6)
	(vp-xor-rr r2 r2)
	(sys-call sys_gettimeofday)
	(vp-pop r1)
	(vpif '(r0 != 0))
		(vp-cpy-ri r0 r1 timeval_sec)
		(vp-cpy-ri r2 r1 timeval_usec)
	(endif))

)
(when (eql *os* 'Linux)

;;;;;;;;;;;;;;;;;;;
; syscall for Linux
;;;;;;;;;;;;;;;;;;;

(defcfun sys-call (_)
	(vp-cpy-cr _ r0)
	(vp-syscall))

(defcvar 'sys_exit 0x3c)
(defcvar 'sys_read 0x0)
(defcvar 'sys_write 0x1)
(defcvar 'sys_open 0x2)
(defcvar 'sys_close 0x3)
(defcvar 'sys_unlink 0x57)
(defcvar 'sys_ftruncate 0x4d)
(defcvar 'sys_stat 0x4)
(defcvar 'sys_mmap 0x9)
(defcvar 'sys_munmap 0xb)
(defcvar 'sys_mprotect 0xa)
(defcvar 'sys_gettimeofday 0x60)
(defcvar 'sys_fcntl 0x48)

(defcvar 'prot_none 0x0)
(defcvar 'prot_read 0x1)
(defcvar 'prot_write 0x2)
(defcvar 'prot_exec 0x4)

(defcvar 'map_shared 0x1)
(defcvar 'map_private 0x2)
(defcvar 'map_fixed 0x10)
(defcvar 'map_anon 0x20)

(defcvar 'o_rdonly 0x0)
(defcvar 'o_wronly 0x1)
(defcvar 'o_rdwr 0x2)
(defcvar 'o_trunc 0x200)
(defcvar 'o_append 0x400)
(defcvar 'o_nonblock 0x800)
(defcvar 'o_creat 0x40)
(defcvar 'o_excl 0x80)
(defcvar 'o_nofollow 0x20000)
(defcvar 'o_cloexec 0x80000)

(defcvar 's_irwxu 0x1c0)
(defcvar 's_irusr 0x100)
(defcvar 's_iwusr 0x80)
(defcvar 's_ixusr 0x40)
(defcvar 's_irwxg 0x38)
(defcvar 's_irgrp 0x20)
(defcvar 's_iwgrp 0x10)
(defcvar 's_ixgrp 0x8)
(defcvar 's_irwxo 0x7)
(defcvar 's_iroth 0x4)
(defcvar 's_iwoth 0x2)
(defcvar 's_ixoth 0x1)
(defcvar 's_isuid 0x800)
(defcvar 's_isgid 0x400)
(defcvar 's_isvtx 0x200)

(defcvar 's_ifmt 0xf000)
(defcvar 's_ifdir 0x4000)
(defcvar 's_ifreg 0x8000)

(defcvar 'f_getfl 0x3)
(defcvar 'f_setfl 0x4)

(defcvar 'stat_mode 0x18)
(defcvar 'stat_fsize 0x30)
(defcvar 'stat_size 0x100)
(defcvar 'stat_mtime 0x58)

(defcfun sys-gettimeofday (_1)
	;time, timezone
	(assign _1 '(r6 r5))
	(sys-call sys_gettimeofday))

)

;;;;;;;;;;;;;;;;
; syscall macros
;;;;;;;;;;;;;;;;

(def-struct 'timeval)
	(long 'sec)
	(long 'usec)
(def-struct-end)

(def-struct 'timezone)
	(int 'minuteswest)
	(int 'dsttime)
(def-struct-end)

(defcfun sys-exit (_1)
	;return code
	(assign _1 '(r6))
	(sys-call sys_exit))

(defcfun sys-read (_1 _2)
	;fd, buffer, length
	(assign _1 '(r6 r5 r2))
	(sys-call sys_read)
	(assign '(r0) _2))

(defcfun sys-read-char (_1 _2)
	;fd
	(assign _1 '(r6))
	(vp-xor-rr r0 r0)
	(vp-push r0)
	(assign '(rsp 1) '(r5 r2))
	(sys-call sys_read)
	(vp-pop r0)
	(assign '(r0) _2))

(defcfun sys-write (_1 _2)
	;fd, string, length
	(assign _1 '(r6 r5 r2))
	(sys-call sys_write)
	(assign '(r0) _2))

(defcfun sys-write-char (_1 _2)
	;fd, char
	(assign _1 '(r6 r5))
	(vp-push r5)
	(assign '(rsp 1) '(r5 r2))
	(sys-call sys_write)
	(vp-free ptr_size)
	(assign '(r0) _2))

(defcfun sys-mmap (_1 _2)
	;addr, len, prot, flags, fd, pos
	(assign _1 '(r6 r5 r2 r9 r7 r8))
	(sys-call sys_mmap)
	(assign '(r0) _2))

(defcfun sys-mprotect (_1 _2)
	;addr, len, prot
	(assign _1 '(r6 r5 r2))
	(sys-call sys_mprotect)
	(assign '(r0) _2))

(defcfun sys-munmap (_1 _2)
	;addr, len
	(assign _1 '(r6 r5))
	(sys-call sys_munmap)
	(assign '(r0) _2))

(defcfun sys-stat (_1 _2)
	;path, buffer
	(assign _1 '(r6 r5))
	(sys-call sys_stat)
	(assign '(r0) _2))

(defcfun sys-open (_1 _2)
	;path, flags, mode
	(assign _1 '(r6 r5 r2))
	(sys-call sys_open)
	(assign '(r0) _2))

(defcfun sys-close (_1 _2)
	;fd
	(assign _1 '(r6))
	(sys-call sys_close)
	(assign '(r0) _2))

(defcfun sys-ftruncate (_1 _2)
	;fd, offset
	(assign _1 '(r6 r5))
	(sys-call sys_ftruncate)
	(assign '(r0) _2))

(defcfun sys-unlink (_1 _2)
	;name
	(assign _1 '(r6))
	(sys-call sys_unlink)
	(assign '(r0) _2))

(defcfun sys-fcntl (_1 _2)
	;fd, cmd, arg
	(assign _1 '(r6 r5 r2))
	(sys-call sys_fcntl)
	(assign '(r0) _2))