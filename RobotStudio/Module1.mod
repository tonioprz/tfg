MODULE Module1
    !***********************************************************
    !
    ! Modulo:  Module1
    !
    ! Descripcion:
    !   <Introduzca la descripcion aqui>
    !
    ! Autor: Antonio
    !
    ! Versi?n: 1.0
    !
    !***********************************************************
    
    VAR socketdev my_socket;
    VAR rawbytes data;
    VAR rawbytes raw_data;
    VAR num estad:=0;
    VAR byte data_buffer;
    VAR bool ok:=true;
    VAR bool okposx:=true;
    VAR bool okposy:=true;
    VAR bool okack:=true;
    VAR bool okfe:=true;
    VAR num i:=1;
    VAR rawbytes receive_string;
    VAR string string1;
    VAR string posx_str;
    VAR string posy_str;
    VAR string fotoele_str;
    VAR num posx;
    VAR num posy;
    VAR byte fotoele;
    VAR num xpos;
    VAR num ypos;
    VAR num fepos;
    VAR num len;
    !***********************************************************
    !
    ! Procedimiento Main
    !
    !   Este es el punto de entrada de su programa
    !
    !***********************************************************
    PROC main()
        SocketClose my_socket;
        
        WaitTime 0.2;
        
        !leer;
        
        mov_abs(20000);
        !mov_dis(10000);
        WaitTime 0.2;
        !leer;
        
        WaitTime 0.5;
    ENDPROC
    
    
    PROC abricomunicacion()        
        SocketCreate my_socket; !crea el socket
        SocketConnect my_socket, "192.168.50.200", 4012;        
    ENDPROC
    
    
    PROC leer()
        abricomunicacion;
        SocketSend my_socket,\Str:="STATUS"; !escribe en la red y lo envia a arduino (donde ser? leido)
        WaitTime 0.1; !espera un tiempo
            
        ClearRawBytes receive_string;
        SocketReceive my_socket \RawData := receive_string,\Time:=WAIT_MAX;
        
        UnpackRawBytes receive_string, 1, string1 \ASCII:=32; !7 es el espacio en caracteres que forman el n?mero
        
        len := StrLen(string1);
        
        xpos        := StrFind(string1, 1, "=");
        ypos        := StrFind(string1, xpos+1, "=");
        fepos       := StrFind(string1, ypos+1, "=");
        
        posx_str    := StrPart(string1, xpos+1, ypos - xpos - 3);
        posy_str    := StrPart(string1, ypos+1, fepos - ypos - 3);
        fotoele_str := StrPart(string1, fepos+1, 1);

        okposx      :=  StrToVal(posx_str,posx);
        okposy      :=  StrToVal(posy_str,posy);
        fotoele :=  StrToByte(fotoele_str);
        
        ClearRawBytes receive_string;
        SocketClose my_socket;
    ENDPROC

    PROC mov_abs(num distancia)
        abricomunicacion;
        SocketSend my_socket,\Str:="M;X=" + ValToStr(distancia) + ";";
        WaitTime 0.1;
        SocketClose my_socket;
    ENDPROC
    
    PROC mov_dis(num distancia)
        abricomunicacion;
        SocketSend my_socket,\Str:="R;X=" + ValToStr(distancia) + ";";
        WaitTime 0.1;
        SocketClose my_socket;
    ENDPROC
ENDMODULE