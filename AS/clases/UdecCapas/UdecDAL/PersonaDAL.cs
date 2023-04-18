using Microsoft.Data.Sqlite;
using UdecBOL;
using static System.Net.Mime.MediaTypeNames;
namespace UdecDAL {
    public class PersonaDAL {
        public PersonaDAL() {
            CreateTable(CreateConnection());
        }
        public bool guardaPersona(PersonaBOL persona) {
            try {
                var conn = CreateConnection();
                SqliteCommand sqlite_cmd;
                sqlite_cmd = conn.CreateCommand();
                sqlite_cmd.CommandText = "INSERT INTO Personas " +
                    "(Rut, Nombre) VALUES('" + persona.rut + "', '" + persona.nombre + "');";
                sqlite_cmd.ExecuteNonQuery();
                conn.Close();
                return true;
            } catch (Exception) {
                return false;
            }
        }
        public List < PersonaBOL > listaPersona() {
            try {
                List < PersonaBOL > lista = new List < PersonaBOL > ();
                var conn = CreateConnection();
                SqliteCommand sqlite_cmd;
                sqlite_cmd = conn.CreateCommand();
                sqlite_cmd.CommandText = "select rut, nombre frmo persons";
                SqliDataReader dr = sqlite_cmd.ExecuteReader();
                while (dr.Read()) {
                    conn.Open();
                    var persona = new PersonaBOL():
                        persona.rut = dr.GetString(0);
                    persona.nombre = dr.GetString(1);
                    lista.Add(persona);
                }
                conn.Close();
                return lista;
            } catch (Exception) {
                return null;
            } finally {
                conn.Close();
            }
        }
        void CreateTable(SqliteConnection conn) {
            conn.Open();
            SqliteCommand sqlite_cmd;
            string Createsql = "CREATE TABLE IF NOT EXISTS Personas (Rut VARCHAR(20), Nombre VARCHAR(20))";
            sqlite_cmd = conn.CreateCommand();
            sqlite_cmd.CommandText = Createsql;
            sqlite_cmd.ExecuteNonQuery();
            conn.Close();
        }
        private SqliteConnection CreateConnection() {
            var pathDB = System.IO.Path.Combine(Environment.CurrentDirectory, "sqlite.db");
            pathDB= @"C:\Users\dfeng2019\Downloads\UdecUI\sqlite.db";
            if (!System.IO.File.Exists(pathDB)) throw new Exception();
            var connection_string = String.Format("Data Source={0}", pathDB);
            SqliteConnection sqlite_conn;
            // Create a new database connection:
            sqlite_conn = new SqliteConnection(connection_string);
            // Open the connection:
            try {
                sqlite_conn.Open();
            } catch (Exception ex) {}
            return sqlite_conn;
        }
    }
}
