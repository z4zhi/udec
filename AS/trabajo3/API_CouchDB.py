import requests
from flask import Flask, request
import grpc
import dbcomponent_pb2
import dbcomponent_pb2_grpc
import datetime

# http://localhost:5984/_utils/#login  admin admin


# grpc
def envio_grpc(document):
	# Enviar datos a la API bativillanos a través de gRPC y Protobuf
	channel = grpc.insecure_channel("localhost:50051")
	stub = dbcomponent_pb2_grpc.DBComponentStub(channel)

	request = dbcomponent_pb2.VillanoRequest()
	request.nombre_villano = document['nombre']
	request.fecha_ingreso = str(datetime.datetime.now())
	
	response = stub.CreateVillano(request)
	print(response.message)
	


app = Flask(__name__)
couchdb_url = 'http://localhost:5984'  # Replace with your CouchDB URL
headers = {'Authorization': 'Basic YWRtaW46YWRtaW4='}
db_name = 'villanos'


@app.route('/api/document', methods=['POST'])
def create_document():
	try:
		document = request.get_json()  # Assuming the request body contains the JSON document
		response = requests.post(f'{couchdb_url}/{db_name}', json=document, headers=headers)
		response.raise_for_status()
		
		envio_grpc(document)
		return 'Document created successfully', 200
	
	except requests.exceptions.RequestException as e:
		return str(e), 500
	except Exception as e:
		return str(e), 400

if __name__ == '__main__':
	app.run()
	

