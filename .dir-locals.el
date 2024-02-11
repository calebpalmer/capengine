((nil .
      ;; set project root
      ((projectile-project-root . "/home/caleb/Sync/Projects/games/capengine")
	      ;; compilation shortcut
       (eval . (local-set-key (kbd "<f12>") 'compile))
       (eval . (setq cap/build-dir (concat projectile-project-root "/build")))
       (eval . (setq compile-command (concat  "cmake --build " (cap/find-dir-locals-folder) "build -- -j4")))
       ;; debug function
       (eval . (defun cap/debug ()
		 (interactive)
		 (progn
		   (gdb (concat "gdb -i=mi --cd " projectile-project-root "/build/src --args tiledtest " (projectile-project-root) "/res/raw/map.tmj")))))

	      (eval . (defun cap/run-tests ()
		 (interactive)
		 (progn
		   (async-shell-command  (concat cap/build-dir "/bin/gtests")))))

       )))
