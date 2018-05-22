(defun my-launch-editor-nodebug ()
  (interactive)
  (shell-command "./editor test_files/map1.cem &")
  )

(defun my-launch-editor-debug ()
  (interactive)
   (gdb "gdb -i=mi --args editor test_files/map1.cem")
   )

(defun my-launch-tests-nodebug ()
  (interactive)
  (shell-command "cd test && ./runtests &")
  )

(defun my-launch-tests-debug ()
  (interactive)
  (gdb "gdb -i=mi test/runtests")
  )


(global-set-key (kbd "C-<f5>") 'my-launch-editor-nodebug)
(global-set-key (kbd "<f5>") 'my-launch-editor-debug)
(global-set-key (kbd "C-<f6>") 'my-launch-tests-nodebug)
(global-set-key (kbd "<f6>") 'my-launch-tests-debug)


;; gdb
(setq gdb-many-windows nil)
(setq gdb-show-main t)
