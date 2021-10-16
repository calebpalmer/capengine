;; look for source directory
(if
    (not (boundp 'cap/source_root_dir))
    (setq cap/source_root_dir default-directory))

(if
    (not (boundp 'cap/build-dir))
    (setq cap/build-dir (concat cap/source_root_dir "/build")))

(setq capengine/source-dir default-directory)

;; stuff to do only once
(defun capengine/launch-nodebug ()
  (interactive)
  (shell-command "./editor test_files/map1.json &")
  )

(defun capengine/launch-debug ()
  (interactive)
  (gdb "gdb -i=mi --args editor test_files/map1.json")
  )

(defun capengine/launch-tests ()
  (interactive)
  (let ((default-directory (concat cap/build-dir  "/capengine/bin")))
    (async-shell-command "./runtests")))

(defun capengine/launch-tests-debug ()
  (interactive)
  (gdb (concat "gdb -i=mi " cap/build-dir "/capengine/test/runtests"))
  )


(defun cap/cmake ()
  (interactive)
  (shell-command (concat "cmake -H" cap/source_root_dir " -B" cap/build-dir  " -DCMAKE_EXPORT_COMPILE_COMMANDS=1 -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=" cap/build-dir "/install && cp " cap/build-dir "/compile_commands.json ."))
  )

;; (defun cap/cmake ()
;;   (interactive)
;;   (shell-command (concat "cmake -H" cap/source_root_dir " -B" cap/build-dir  " -DCMAKE_EXPORT_COMPILE_COMMANDS=1 -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=" cap/build-dir "/install -DCMAKE_C_COMPILER=/usr/bin/clang -DCMAKE_CXX_COMPILER=/usr/bin/clang++ && cp " cap/build-dir "/compile_commands.json ."))
;;   )

(defun cap/cmake-install ()
  (interactive)
  (shell-command (concat "cmake --build " cap/build-dir " --target install"))
  )

(defun cap/conan-install ()
  (interactive)
  (let ((default-directory cap/build-dir))
    (shell-command (concat "conan install " cap/source_root_dir " --build missing"))))

(setq cap/capengine-functions t)


(setq compile-command (concat "cmake --build " cap/build-dir))

(global-set-key (kbd "<f12>") 'compile)
(global-set-key (kbd "C-<f12>") 'cap/cmake-install)

;; gdb
(setq gdb-many-windows nil)
(setq gdb-show-main t)
