import hashlib
import datetime
import asyncio
from websockets.sync.client import connect
import socket
from flask import Flask, request, jsonify
from flask_jwt_extended import JWTManager, create_access_token, jwt_required, get_jwt_identity
from pymongo import MongoClient
from pymongo.server_api import ServerApi
from bson.objectid import ObjectId




app = Flask(__name__)
jwt = JWTManager(app)
app.config['JWT_SECRET_KEY'] = 'aassddaasdd123'
app.config['JWT_ACCESS_TOKEN_EXPIRES'] = datetime.timedelta(days=1)
uri = "mongodb+srv://gonpec:j4yj1lFv94OcCzdC@cluster0.dcycxpo.mongodb.net/?retryWrites=true&w=majority"








client = MongoClient(uri, server_api=ServerApi('1'))
db = client["demo"]
users_collection = db["users"]


@app.route("/api/v1/users", methods=["POST"])
def register():
	new_user = request.get_json() # store the json body request
	new_user["password"] = hashlib.sha256(new_user["password"].encode("utf-8")).hexdigest() # encrpt password
	doc = users_collection.find_one({"username": new_user["username"]}) # check if user exist
	if not doc:
		users_from_db = users_collection.find({})
		total =len(list(users_from_db.clone()))
		test(str(total))
		return jsonify({'msg': 'User created successfully'}), 201
	else:
		return jsonify({'msg': 'Username already exists'}), 409



@app.route("/api/v1/login", methods=["POST"])
def login():
	login_details = request.get_json() # store the json body request
	user_from_db = users_collection.find_one({'username': login_details['username']})  # search for user in database

	if user_from_db:
		encrpted_password = hashlib.sha256(login_details['password'].encode("utf-8")).hexdigest()
		if encrpted_password == user_from_db['password']:
			access_token = create_access_token(identity=user_from_db['username']) # create jwt token
			return jsonify(access_token=access_token), 200

	return jsonify({'msg': 'The username or password is incorrect'}), 401




@app.route("/api/v1/user", methods=["GET"])
@jwt_required()
def profile():
	current_user = get_jwt_identity() # Get the identity of the current user
	user_from_db = users_collection.find_one({'username' : current_user})
	if user_from_db:
		del user_from_db['_id'], user_from_db['password'] # delete data we don't want to return
		return jsonify({'profile' : user_from_db }), 200
	else:
		return jsonify({'msg': 'Profile not found'}), 404


@app.route("/api/v1/user/<user_id>", methods=['DELETE'])
@jwt_required()
def remove_user(user_id):
    try:
        delete_user = users_collection.delete_one({'_id': ObjectId(user_id)})
        if delete_user.deleted_count > 0 :
            return "", 204
        else:
            return "", 404
    except:
        return "", 500


def test(mensaje):
    print("Enviando al socket")
    with connect("ws://localhost:8091") as websocket:
        websocket.send(mensaje)
        message = websocket.recv()
        print(f"Received: {message}")
	



def sendData(mensaje):
	mi_socket = socket.socket()
	mi_socket.connect(('localhost',8091))
	print("Enviando a socket:" + mensaje)
	mi_socket.send(mensaje.encode())
	mi_socket.close();
#   async with websockets.connect("ws://localhost:8091") as websocket:
#         await websocket.send(mensaje)
        # response = await websocket.recv()
        # print(response)




@app.route("/api/v1/usersAll", methods=["GET"])
@jwt_required()
def usersAll():
	current_user = get_jwt_identity() # Get the identity of the current user
	users_from_db = users_collection.find({})
	total =len(list(users_from_db.clone()))
	sendData(str(total))
	print(f"total: {total}")
	data =[]

	if total>0:
		for doc in users_from_db:
			doc["_id"] = str(doc["_id"])
			data.append(doc)
	
	#	return jsonify({'profile' : users_from_db }), 200
	#	return jsonify(list(users_from_db))
		return jsonify(data)
	else:
		return jsonify({'msg': '0 records'}), 404


if __name__ == '__main__':
	app.run(debug=True)