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

using Microsoft.Data.Sqlite;
using UdecBOL;

namespace UdecDAL
{
    public class PersonaDAL
    {
        public PersonaDAL() {
            CreateTable(CreateConnection());
        }

        public bool guardaPersona(PersonaBOL persona) { 
        
            try
            {
            var conn = CreateConnection();
            SqliteCommand sqlite_cmd;
            sqlite_cmd = conn.CreateCommand();
            sqlite_cmd.CommandText = "INSERT INTO Personas " +
                "(Rut, Nombre) VALUES('"+ persona.rut + "', '"+ persona.nombre +"');";
            sqlite_cmd.ExecuteNonQuery();
                return true;
            }
            catch (Exception)
            {

                return false;
            }
        }

         void CreateTable(SqliteConnection conn)
        {

            SqliteCommand sqlite_cmd;
            string Createsql = "CREATE TABLE IF NOT EXISTS Personas (Rut VARCHAR(20), Nombre VARCHAR(20))";
            sqlite_cmd = conn.CreateCommand();
            sqlite_cmd.CommandText = Createsql;
            sqlite_cmd.ExecuteNonQuery();
         

        }

        private   SqliteConnection CreateConnection()
        {

            SqliteConnection sqlite_conn;
            // Create a new database connection:
            sqlite_conn = new SqliteConnection("Data Source=database.db; Version = 3; New = True; Compress = True; ");
         // Open the connection:
         try
            {
                sqlite_conn.Open();
            }
            catch (Exception ex)
            {

            }
            return sqlite_conn;
        }

    }
}

En BOL, click derecho, agregar clase: PersonaBOL.CS  

public..{  
prob (tab) (tab)  
public string rut {get;set;}    
public nombre rut {get;set;}  
}

En DAL  
 - public bool guardaPersona(PersonaBOL persona)
 - persona.rut, persona.nombre

En BL, click derecho, agregar clase: PersonaBL.cs

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using UdecBOL;
using UdecDAL;

namespace UdecBL
{
    public class PersonaBL
    {
        PersonaDAL personaDAL;
        public PersonaBL()
        {
            personaDAL = new PersonaDAL();

        }

        public bool guardaPersona(PersonaBOL persona)
        {
            //Agregamos llamada a cierta logica
            if (persona.nombre.Contains("ñ"))
            {
                return false;
            }

            //luego llamamos a nuestro DAL
            try
            {
                personaDAL.guardaPersona(persona);
                return true;
            }
            catch (Exception)
            {

                return false;
            }

        }
    }
}

en pages  
en index.cshtml

