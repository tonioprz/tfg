MODULE Module1
    !***********************************************************
    !
    ! M�dulo:  Module1
    !
    ! Descripci�n:
    !   <Introduzca la descripci�n aqu�>
    !
    ! Autor: Antonio
    !
    ! Versi�n: 1.0
    !
    !***********************************************************
    
    VAR socketdev my_socket;
    VAR rawbytes data;
    VAR rawbytes raw_data;
    VAR byte nuevo:=0;
    VAR num n;
    VAR string n2;
    var num float;
    var num float2;
    VAR byte data_buffer;
    VAR bool ok:=true;
    VAR num i:=1; 
    VAR rawbytes receive_string;
    VAR string string1;
    !***********************************************************
    !
    ! Procedimiento Main
    !
    !   Este es el punto de entrada de su programa
    !
    !***********************************************************
    PROC main()
        !A�ada aqu� su c�digo
        SocketCreate my_socket; !crea el socket
        SocketConnect my_socket, "192.168.50.200", 4012;
        
        WHILE ok=TRUE DO
            SocketSend my_socket,\Str:="Hola holita vecinito"; !escribe en la red y lo envia a arduino (donde ser� leido)
            WaitTime 0.5; !espera un tiempo
            SocketReceive my_socket \RawData := receive_string,\Time:=WAIT_MAX;

            UnpackRawBytes receive_string, 1, string1 \ASCII:=15; !7 es el espacio en caracteres que forman el n�mero
            UnpackRawBytes receive_string, 8, n2 \ASCII:=8;
            ok:=StrToVal(string1,float);
            ok:=StrToVal(n2,float2);
        ENDWHILE
        !TPWrite "Instruction SocketClose has been executed";


        !UnpackRawBytes data,1,nuevo,\ASCII:=15;
        !!UnpackRawBytes data,2,nuevo,\ASCII:=1;
        !UnpackRawBytes data,3,nuevo1,\ASCII:=1;
        !UnpackRawBytes data,4,nuevo2,\ASCII:=1;

        SocketClose my_socket; !cierra el socket
        ok:=TRUE;
    ENDPROC
ENDMODULE