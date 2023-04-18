using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using UdecBOL;
using UdecDAL;
namespace UdecBL {
    public class PersonaBL {
        PersonaDAL personaDAL;
        public PersonaBL() {
            personaDAL = new PersonaDAL();
        }
        public List < PersonaBOL > listaPersonas() {
            try {
                return personaDAL.listaPersona();
            } catch (Exception) {
                return null;
            }
        }
        public bool guardaPersona(PersonaBOL persona) {
            //Agregamos llamada a cierta logica
            if (persona.nombre.Contains("ñ")) {
                return false;
            }
            //luego llamamos a nuestro DAL
            try {
                personaDAL.guardaPersona(persona);
                return true;
            } catch (Exception) {
                return false;
            }
        }
    }
}
