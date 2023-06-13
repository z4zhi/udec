Crear proyecto VS: Servicio gRPC de ASP .NET Core  
.NET 7.0  

en Protos, agregar nuevo elemento -> archivo de bufer de procotolo  
sumador.proto: wsp

en Services, agregar clase  
UdecService.cs: wsp  
public class UdecService : Sumador.SumadorBase

en GrpcService-> editar archivo de proyecto:  
pegar un itemgroup de wsp, reemplaza el que tiene solo el greet

