;;; Directory Local Variables
;;; For more information see (info "(emacs) Directory Variables")

((nil
  ;;(compile-command . "gtags && cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=1 . && rc -J && make"))
  ;;(compile-command . "make -j4"))
  ;;(compile-command . "cmake --build build")
  (if
      (not (boundp 'cap/project_loaded))
      (progn (load-file "./init.el") (setq cap/project-loaded "t"))))
 (c++-mode
  (tab-width . 2))
 ((c-mode . ((mode . c++)))) 
 )

