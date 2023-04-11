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
En PersonaDAL.cs  

using Microsoft.Data.SQLite;
... (codigo en wsp)

En BOL, agregar clase: PersonaBOL.CS  

public..{  
prob (tab) (tab)  
public string rut {get;set;}    
public nombre rut {get;set;}  
}

En DAL  
 - public bool guardaPersona(PersonaBOL persona)
 - persona.rut, persona.nombre
