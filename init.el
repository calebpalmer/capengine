;; look for source directory
(if
    (not (boundp 'cap/source_root_dir))
    (setq cap/source_root_dir default-directory))

(if
    (not (boundp 'cap/build-dir))
    (setq cap/build-dir (concat cap/source_root_dir "/debug")))

;; stuff to do only once
(if
    (not (boundp 'cap/capengine-functions))
    (progn
      (defun my-launch-nodebug ()
	(interactive)
	(shell-command "./editor test_files/map1.json &")
	)

      (defun my-launch-debug ()
	(interactive)
	(gdb "gdb -i=mi --args editor test_files/map1.json")
	)

      (defun my-launch-tests-nodebug ()
	(interactive)
	(shell-command "cd test && ./runtests &")
	)

      (defun my-launch-tests-debug ()
	(interactive)
	(gdb "gdb -i=mi test/runtests")
	)

      (defun cap/cmake ()
	(interactive)
	(shell-command (concat "cmake -H" cap/source_root_dir " -B" cap/build-dir  " -DCMAKE_EXPORT_COMPILE_COMMANDS=1 -DCMAKE_BUILD_TYPE=Debug && cp " cap/build-dir "/compile_commands.json ."))
	)

      (defun cap/cmake-install ()
	(interactive)
	(shell-command (concat "cmake --build " cap/build-dir " --target install"))
	)

      (setq cap/capengine-functions t)
      ))

(setq compile-command (concat "cmake --build " cap/build-dir " -- -j4 && cp " cap/build-dir "/compile_commands.json ."))

(local-set-key (kbd "C-<f5>") 'my-launch-nodebug)
(local-set-key (kbd "<f5>") 'my-launch-debug)
(local-set-key (kbd "C-<f6>") 'my-launch-tests-nodebug)
(local-set-key (kbd "<f6>") 'my-launch-tests-debug)
(local-set-key (kbd "<f12>") 'compile)
(local-set-key (kbd "C-<f12>") 'cap/cmake-install)
(local-set-key (kbd "<f10>") 'gdb-many-windows)

;; gdb
(setq gdb-many-windows nil)
(setq gdb-show-main t)
