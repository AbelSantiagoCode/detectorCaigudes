#!/usr/bin/python
import socket
import select 
import sys
import requests


"""
Aquest modul implementa la part del servidor per atendre multiples clients,
amb el fi d'establir una comunicacio amb el client (AVR-ARDUINO) i un bot de telegram.


Notes:
======
	1. La clase ServerPy, el qual fa de rol de servidor, espera rebre les 
	dades del client (AVR_ARDUINO).


	2. Si un client vol tanca la connexio TCP de forma correcte, s'espera que 
	el Client envie la data-keyword "exit". De la mateixa forma, quan el Servidor vol
	tancar la connexio s'ha d'escriure pel terminal la data-keyword "exit" or clicar "Crtl+C", cal dir
	que si el servidor tanca la connexio envia "exit" a tots els clients connectats
	amb el fi d'avisar el tancament de la connexio.

"""

class ServerPy(object):
	"""ServerChat which uses TCP protocol"""
	def __init__(self, HOST, PORT, id_bot, token_bot, url_bot):
		self.HOST = HOST
		self.PORT = PORT
		self.id_bot = id_bot
		self.token_bot = token_bot
		self.url_bot = url_bot
		self.NClient    = 1
		self.SockClients = []

		try:
			self.SERVER  = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
			self.SERVER.bind((HOST,PORT))
			self.SERVER.listen(self.NClient)
		except socket.error, e:
			print("Error creating socket: "+str(e))
			sys.exit(1)		
		self.SelectList = [self.SERVER,sys.stdin]

	def AppendClient(self,Client):
		self.SelectList.append(Client)
		self.SockClients.append(Client)

	def RemoveClient(self,Client):
		self.SelectList.remove(Client)
		self.SockClients.remove(Client)

	def CloseClient(self,Client):
		self.RemoveClient(Client)
		Client.close()

	def CloseClients(self):
		print("CloseClients")
		for Client in self.SockClients:
			self.CloseClient(Client)
			
	def CloseServer(self):
		print("CloseServer")
		self.SendData2Clients("exit")
		self.CloseClients()
		self.SERVER.close()
		
		sys.exit(1)

	def SendData2Clients(self,Data):
		for ClientSock in self.SockClients:
			ClientSock.sendall(Data)

	def ProcessKeyBoardData(self,Data):
		if (Data == "exit") or (Data == "exit\n"):
			self.CloseServer()
		else:      #For DEBBUG
			self.SendData2Clients(Data)




	def ProcessData(self,Data,Client):
		if (Data == "exit\n") or (Data == "exit"):
			self.CloseClient(Client)
		else:
			print(Data)			
			self.send_message_telegram(Data)
			


	def ServiceClient(self,Client):
		"""Prints/Insert DataBase Data sended by each client"""
		try:
			DataClient = Client.recv(1024)

			self.ProcessData(DataClient,Client)
	
		except Exception as e:
			self.CloseClient(Client)

	def send_message_telegram(self,text):
	
		url= self.url_bot + self.token_bot + "/sendMessage"
		params = {
			'chat_id': self.id_bot,
 
			'text' : text
		}
 
		requests.post(url, params=params)


	def Run(self):

		while True:
			try: 
				rlist, wlist, xlist = select.select(self.SelectList,[],[])
				
				if rlist[0] == self.SelectList[0]:
					Client,Addr = self.SERVER.accept()   	#Accept Client 
					print("Connection from: "+str(Addr))
					self.AppendClient(Client)

				elif rlist[0] != self.SelectList[1]: 		# CLIENT 
					ClientSock = rlist[0]
					self.ServiceClient(ClientSock)

				elif rlist[0] == self.SelectList[1]: 		# KEYBOARD -TERMINAL 
					DataKeyB = sys.stdin.readline()
					self.ProcessKeyBoardData(DataKeyB)
				else:
					print("\nERROR")
			except Exception as e:
				print("Error Interrupt")
				self.CloseServer()
			except KeyboardInterrupt :
				print("Keyboard Interrupt")
				self.CloseServer()
		self.CloseServer()





if __name__ == '__main__':

	Server = ServerPy("192.168.43.50",5000, "-1001271064871","615582162:AAGgt4fbrWwCtNCzEltixeg4r1_-WXay2AI","https://api.telegram.org/bot") #HOST,PORT,id_bot, token_bot, url_bot
	Server.Run()
	
