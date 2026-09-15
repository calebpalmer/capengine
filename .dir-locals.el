((nil .
      ;; set project root
      ((projectile-project-root . "/home/caleb/Sync/Projects/games/capengine")
       ;; compilation shortcut
       (eval . (local-set-key (kbd "<f12>") 'compile))
       (eval . (setq cap/build-dir (concat projectile-project-root "/build")))
       (eval . (setq compile-command (concat  "cmake --build " (cap/find-dir-locals-folder) "build")))
       (eval . (setq gdb-command-name (concat "gdb -i=mi " cap/build-dir "/bin/rps")))
       ;; debug functions
       (eval . (defun cap/debug ()
		 (interactive)
		 (progn
		   (setenv "CP_ASSETFOLDER" (concat cap/build-dir "/postapoc_resources"))
		   (gdb (concat "gdb -i=mi --cd " cap/build-dir "/bin --args postapoc")))))

       (eval . (defun cap/debug-tests ()
		 (interactive)
		 (progn
		   (gdb (concat "gdb -i=mi --cd " projectile-project-root "/build/bin --args gtests")))))

       (eval . (defun cap/run-tests ()
		 (interactive)
		 (progn
		   (async-shell-command  (concat cap/build-dir "/bin/gtests")))))
       ))
 (c-mode . ((c-basic-offset . 4)         ;; Set basic indentation to 4 spaces
	    (indent-tabs-mode . nil)     ;; Use spaces instead of tabs
	    (c-default-style . "capengine")
	    (c-file-style . "capengine")))
 (c++-mode . ((c-basic-offset . 4)        ;; Set basic indentation to 4 spaces
	      (indent-tabs-mode . nil)    ;; Use spaces instead of tabs
	      (c-default-style . "capengine")
	      (c-file-style . "capengine"))))
