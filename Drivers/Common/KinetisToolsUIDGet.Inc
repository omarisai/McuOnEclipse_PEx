%- AUTOREGENERATE If you remove this line, this file cannot be rewrited (by default)
%ifndef CommentLine
%{
%endif CommentLine
%;** ===================================================================
%include Common\GeneralMethod.inc (UIDGet)
%;**     Description :
%;**         Return the 128bit UID of the device
%include Common\GeneralParameters.inc(27)
%;**       * uid%Paruid %>27 - Pointer to 
%;**     Returns     :
%;**         ---%RetVal %>27 - Error code
%include Common\GeneralDamage.inc
%;** ===================================================================
%ifndef CommentLine
%}
%endif CommentLine