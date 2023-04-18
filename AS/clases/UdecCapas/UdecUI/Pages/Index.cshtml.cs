using Microsoft.AspNetCore.Mvc;
using Microsoft.AspNetCore.Mvc.RazorPages;
using UdecBL;
namespace UdecUI.Pages {
    public class IndexModel: PageModel {
        private readonly ILogger < IndexModel > _logger;
        public IndexModel(ILogger < IndexModel > logger) {
            _logger = logger;
        }
        public void OnGet() {
            ViewData["lista"] = new PersonaBL().listaPersonas();
        }
        public IActionResult OnPost(string tx_rut, string tx_nombre) {
            PersonaBL bl = new PersonaBL();
            bl.guardaPersona(new UdecBOL.PersonaBOL() {
                rut = tx_rut, nombre = tx_nombre
            });
            ViewData["lista"] = new PersonaBL().listaPersonas();
            return Page();
        }
    }
}