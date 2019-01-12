;; look for source directory
(if
    (not (boundp 'cap/source_root_dir))
    (setq cap/source_root_dir default-directory))

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
  (shell-command (concat "cmake -H" cap/source_root_dir " -B" cap/source_root_dir  "build -DCMAKE_EXPORT_COMPILE_COMMANDS=1"))
  )

(setq compile-command (concat "cmake --build " cap/source_root_dir "build -j2"))

(global-set-key (kbd "C-<f5>") 'my-launch-editor-nodebug)
(global-set-key (kbd "<f5>") 'my-launch-editor-debug)
(global-set-key (kbd "C-<f6>") 'my-launch-tests-nodebug)
(global-set-key (kbd "<f6>") 'my-launch-tests-debug)
(global-set-key (kbd "C-<f12>") 'cap/cmake)
(global-set-key (kbd "<f10>") 'gdb-many-windows)

;; gdb
(setq gdb-many-windows nil)
(setq gdb-show-main t)

(projectile-mode)
