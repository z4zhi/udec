Crear proyecto VS: Servicio gRPC de ASP .NET Core  
.NET 7.0  

en Protos, agregar nuevo elemento -> archivo de bufer de procotolo  
sumador.proto: 
```
syntax = "proto3";

option csharp_namespace = "GrpcGreeter";

package sumar;



service Sumador {
  rpc SumaDosNumeros (SumaRequest) returns (SumaReply);
}

message SumaRequest{
   int32 numero_uno = 1;
   int32 numero_dos = 2;
}

message SumaReply{
   int32 resultado = 1;
}
```
en Services, agregar clase  
UdecService.cs: 
```
public class UdecService :Sumador.SumadorBase
    {
        private readonly ILogger<GreeterService> _logger;
        public UdecService(ILogger<GreeterService> logger)
        {
            _logger = logger;
        }

        public override Task<SumaReply> SumaDosNumeros(SumaRequest request, ServerCallContext context)
        {
            return Task.FromResult(new SumaReply
            {
                Resultado = request.NumeroUno + request.NumeroDos
            }); ;
        }
    }
```
en GrpcService-> editar archivo de proyecto:  
pegar un itemgroup de wsp, reemplaza el que tiene solo el greet  
```
<ItemGroup>
    <Protobuf Include="Protos\greet.proto" GrpcServices="Server" />
    <Protobuf Include="Protos\sumador.proto" GrpcServices="Server" />
  </ItemGroup>
```
```
using Grpc.Core;
using GrpcGreeter;
```
run

