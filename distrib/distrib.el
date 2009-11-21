;;; Utility Emacs Lisp functions for w32los distribution .
;;; Helmut Steeb 2005-11-29
;;; $Id: distrib.el 1090 2006-11-27 19:49:32Z helmut $
;;;
;;; Contents:
;;;

(defun adapt()
  (interactive)

  (goto-char 1)
  (query-replace-regexp "4.0.7" "4.0.9")
  (query-replace-regexp "4.0.8" "4.0.9")

  (goto-char 1)
  (query-replace-regexp "06" "07")
)
