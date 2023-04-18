Aplicacion web ASP.NET (.NET Framework)
C# Web -> Aplicacion web ASP.NET (.NET Framework)
#
Aplicacion web de ASP.NET Core  
 - Nombre: UdecUI  

En Solucion "UdecUI", click derecho, agregar: Nuevo Proyecto
- Biblioteca de clases  
  - Nombre: UdecBL
- Biblioteca de clases  
  - Nombre: UdecDAL
- Biblioteca de clases  
  - Nombre: UdecBOL

En UI, click derecho, agregar: Referencia del proyecto.
- BL, BOL

En los otros:
- BL -> DAL, BOL
- DAL-> BD, BOL

En DAL click derecho -> Admin. de paquetes NuGet -> SQLite (instalar)  

En BOL, click derecho, agregar clase: PersonaBOL.CS  

public..{  
prob (tab) (tab)  
public string rut {get;set;}    
public nombre rut {get;set;}  
}


en solucion, agregar: nuevo proyecto.  
ASP.NET Core vacio
 - nombre: UdecAPI
 - .NET 7.0

en udecapi, agregar: referencia de proyecto
 - udecbl
 - udecbol

en udecui, click derecho, poryecto de inicio multiple udeui y udecapi

#ngrok
