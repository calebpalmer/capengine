;;; Directory Local Variables
;;; For more information see (info "(emacs) Directory Variables")

((nil
  (compile-command . "gtags && cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=1 . && rc -J && make"))
  ;;(compile-command . "cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=1 . && make"))
 (c++-mode
  (tab-width . 2))
  (c-basic-offset . 2) 
 (c-mode
  (c-basic-offset . 2)
  (tab-width . 2)))

