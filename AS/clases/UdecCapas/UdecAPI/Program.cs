var builder = WebAplication.CreateBuilder(args);
var app = builder.Build();
app.Mapget("/", () => "Hola mundo!");
app.Mapget("/PersonasV1", () => {
    List < PersonaBOL > lista = new PersonaBL().listaPersonas();
    return lista;
});
app.Run();