import grpc
from concurrent import futures
import dbcomponent_pb2
import dbcomponent_pb2_grpc
import pymongo


# python -m grpc_tools.protoc -I. --python_out=. --grpc_python_out=. dbcomponent.proto
class DBComponentServicer(dbcomponent_pb2_grpc.DBComponentServicer):
	def __init__(self):
		# Establecer la conexión con MongoDB
		self.client = pymongo.MongoClient('mongodb://admin:admin@localhost:27017')  # Replace with your MongoDB connection string
		self.db = self.client['villanos']
		self.collection = self.db['villanos']

	def CreateVillano(self, request, context):
		# Insertar el villano en la base de datos de MongoDB
		villano = {
			'nombre_villano': request.nombre_villano,
			'fecha_ingreso': request.fecha_ingreso
		}
		self.collection.insert_one(villano)
		
		# Enviar una respuesta de éxito
		return dbcomponent_pb2.VillanoResponse(message='Villano created successfully')


def serve():
	server = grpc.server(futures.ThreadPoolExecutor(max_workers=10))
	dbcomponent_pb2_grpc.add_DBComponentServicer_to_server(DBComponentServicer(), server)
	server.add_insecure_port('[::]:50051')
	server.start()
	print("Server started. Listening on port 50051...")
	server.wait_for_termination()


if __name__ == '__main__':
	serve()
