;; look for source directory
(if
    (not (boundp 'cap/source_root_dir))
    (setq cap/source_root_dir default-directory))

(if
    (not (boundp 'cap/capengine-functions))
    (progn
      (defun my-launch-editor-nodebug ()
	(interactive)
	(shell-command "./editor test_files/map1.json &")
	)

      (defun my-launch-editor-debug ()
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
	(shell-command (concat "cmake -H" cap/source_root_dir " -B" cap/source_root_dir  "build -DCMAKE_EXPORT_COMPILE_COMMANDS=1 -DCMAKE_BUILD_TYPE=Debug"))
	)
      (setq cap/capengine-functions t)
      ))

(setq compile-command (concat "cmake --build " cap/source_root_dir "build -- -j4"))

(local-set-key (kbd "C-<f5>") 'my-launch-editor-nodebug)
(local-set-key (kbd "<f5>") 'my-launch-editor-debug)
(local-set-key (kbd "C-<f6>") 'my-launch-tests-nodebug)
(local-set-key (kbd "<f6>") 'my-launch-tests-debug)
(local-set-key (kbd "<f12>") 'compile)
(local-set-key (kbd "C-<f12>") 'cap/cmake)
(local-set-key (kbd "<f10>") 'gdb-many-windows)

;; gdb
(setq gdb-many-windows nil)
(setq gdb-show-main t)

(projectile-mode)
