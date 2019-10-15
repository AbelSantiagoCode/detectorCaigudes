import sys
import time 




class FSM_Posicio(object):
	
	def __init__(self):
		self.values_range = [0,0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9,1]
		self.x_range = [0,0,0,0,0,0,0,0,0,0,0]
		self.y_range = [0,0,0,0,0,0,0,0,0,0,0]
		self.z_range = [0,0,0,0,0,0,0,0,0,0,0]

		self.estat = 1
		self.posicio_anterior = 'desconegut'
		self.posicio_actual = 'desconegut'

	def reset_buffer(self):
		self.x_range = [0,0,0,0,0,0,0,0,0,0,0]
		self.y_range = [0,0,0,0,0,0,0,0,0,0,0]
		self.z_range = [0,0,0,0,0,0,0,0,0,0,0]

	def sendEvent(self,event):
		self.run_FSM(event)

	def setBuffers(self,x,y,z):
		self.x_range = x
		self.y_range = y
		self.z_range = z
		

	def update_posicio(self):
		Index_maxX = self.x_range.index(max(self.x_range)) 
		Index_maxY = self.y_range.index(max(self.y_range))
		Index_maxZ = self.z_range.index(max(self.z_range))
		maxX = self.values_range[Index_maxX]
		maxY = self.values_range[Index_maxY]
		maxZ = self.values_range[Index_maxZ]
		#print(self.x_range,self.y_range,self.z_range)
		print(maxX,maxY,maxZ)
		if maxX>=0.75 and maxY<=0.4 and maxZ<=0.4:
			return 'erecte'
		elif maxX<=0.4 and maxY<=0.4 and maxZ>=0.75:
			return 'tumbat'
		elif maxX<=0.4 and maxY>=0.75 and maxZ<=0.4:
			return 'tumbat'
		else:
			return 'desconegut'

	def check_fall(self):
		if self.posicio_anterior == 'erecte' and self.posicio_actual == 'tumbat':
			print("!!!!!!!! DANGER : CAIGUDA DETECTADA !!!!!!!!")
		elif self.posicio_anterior == 'desconegut' and self.posicio_actual == 'tumbat':
			print("******** ALERT : POSSIBLE CAIGUDA DETECTADA ********")
		elif self.posicio_anterior == 'desconegut' and self.posicio_actual == 'desconegut':
			print("######## CHECK : SITUACIO EXTRANYA ########")

	def run_FSM(self,event):
		if self.estat == 1:
			if event == 'time_out':
				self.posicio_anterior = self.update_posicio()
				print(self.posicio_anterior)
				self.reset_buffer()
			
			elif event == 'threshold':
				self.estat = 2
				self.reset_buffer()
		else : 
			if event == 'time_out':
				self.estat = 1
				self.posicio_actual = self.update_posicio()
				print(self.posicio_actual)
				self.reset_buffer()
				self.check_fall()



class FSM_Mostrejar(object):
	
	def __init__(self, FSM_Posicio):
		self.FSM_Posicio = FSM_Posicio
		self.state = 1
		self.time_out = 199   #200 cicles de rellotge, el qual equival a 2 segons abans i despres de detectar el THRESHOLD(REBOT D'ACCELERACIO)
		self.finestra = 0
		self.fd = None #File Descriptor of Samples
		#values_range = [0,0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9,1]
					  #[1,2,3,4,5,6,7,8,9,10,11]	
		self.x_range = [0,0,0,0,0,0,0,0,0,0,0]
		self.y_range = [0,0,0,0,0,0,0,0,0,0,0]
		self.z_range = [0,0,0,0,0,0,0,0,0,0,0]

	def reset_buffer(self):
		self.x_range = [0,0,0,0,0,0,0,0,0,0,0]
		self.y_range = [0,0,0,0,0,0,0,0,0,0,0]
		self.z_range = [0,0,0,0,0,0,0,0,0,0,0]

	def reset_finestra(self):
		self.finestra = 0

	def first_threshold(self,x,y,z):
		if x >= 0.7 and y >= 0.7:
			return True
		elif x >= 0.7 and z >= 0.7:
			return True
		elif y >= 0.7 and z >= 0.7:
			return True
		else :
			return False
			
	def classificar_rangX(self,x):
		if x >= 1:
			self.x_range[10] = self.x_range[10] + 1
		elif x >= 0.9 :
			self.x_range[9] = self.x_range[9] + 1
		elif x >= 0.8 :
			self.x_range[8] = self.x_range[8] + 1
		elif x >= 0.7 :
			self.x_range[7] = self.x_range[7] + 1
		elif x >= 0.6 :
			self.x_range[6] = self.x_range[6] + 1
		elif x >= 0.5 :
			self.x_range[5] = self.x_range[5] + 1
		elif x >= 0.4 :
			self.x_range[4] = self.x_range[4] + 1
		elif x >= 0.3 :
			self.x_range[3] = self.x_range[3] + 1
		elif x >= 0.2 :
			self.x_range[2] = self.x_range[2] + 1
		elif x >= 0.1 :
			self.x_range[1] = self.x_range[1] + 1
		elif x >= 0 :
			self.x_range[0] = self.x_range[0] + 1

	def classificar_rangY(self,y):
		if y >= 1:
			self.y_range[10] = self.y_range[10] + 1
		elif y >= 0.9 :
			self.y_range[9] = self.y_range[9] + 1
		elif y >= 0.8 :
			self.y_range[8] = self.y_range[8] + 1
		elif y >= 0.7 :
			self.y_range[7] = self.y_range[7] + 1
		elif y >= 0.6 :
			self.y_range[6] = self.y_range[6] + 1
		elif y >= 0.5 :
			self.y_range[5] = self.y_range[5] + 1
		elif y >= 0.4 :
			self.y_range[4] = self.y_range[4] + 1
		elif y >= 0.3 :
			self.y_range[3] = self.y_range[3] + 1
		elif y >= 0.2 :
			self.y_range[2] = self.y_range[2] + 1
		elif y >= 0.1 :
			self.y_range[1] = self.y_range[1] + 1
		elif y >= 0 :
			self.y_range[0] = self.y_range[0] + 1

	def classificar_rangZ(self,z):
		if z >= 1:
			self.z_range[10] = self.z_range[10] + 1
		elif z >= 0.9 :
			self.z_range[9] = self.z_range[9] + 1
		elif z >= 0.8 :
			self.z_range[8] = self.z_range[8] + 1
		elif z >= 0.7 :
			self.z_range[7] = self.z_range[7] + 1
		elif z >= 0.6 :
			self.z_range[6] = self.z_range[6] + 1
		elif z >= 0.5 :
			self.z_range[5] = self.z_range[5] + 1
		elif z >= 0.4 :
			self.z_range[4] = self.z_range[4] + 1
		elif z >= 0.3 :
			self.z_range[3] = self.z_range[3] + 1
		elif z >= 0.2 :
			self.z_range[2] = self.z_range[2] + 1
		elif z >= 0.1 :
			self.z_range[1] = self.z_range[1] + 1
		elif z >= 0 :
			self.z_range[0] = self.z_range[0] + 1

	def sendEvent_FSMPosicio(self,event):
		self.FSM_Posicio.sendEvent(event)

	def setBuffers_FSMPosicio(self,x,y,z):
		self.FSM_Posicio.setBuffers(x,y,z)
	
	def set_ADXL(self):
		self.fd = open(sys.argv[1],"r")
		

	def read_ADXL(self):
		try:
			line = self.fd.readline()
			line = line.replace('\n','')
			line = line.replace('\r','')
			if line != '':
				dades = line.split(' ')
				#print(dades)
				return [abs(float(dades[0])),abs(float(dades[1])),abs(float(dades[2]))]
			else:
				self.fd.close()
				sys.exit()

		except Exception as e:
			print("PROGRAMA ACABAT")
			sys.exit()
	
	def run_FSM(self):
		while True:
			x,y,z = self.read_ADXL()
			self.classificar_rangX(x)
			self.classificar_rangY(y)
			self.classificar_rangZ(z)

			if self.finestra == self.time_out:
				self.setBuffers_FSMPosicio(self.x_range,self.y_range,self.z_range)
				self.reset_buffer()
				self.reset_finestra()
				self.sendEvent_FSMPosicio('time_out')
			
			elif self.first_threshold(x,y,z):
				self.state = 2
				self.reset_buffer()
				self.reset_finestra()
				self.sendEvent_FSMPosicio('threshold')

			self.finestra = self.finestra + 1
				
			"""
			if self.state == 1 :
			else : 
				if self.finestra == self.time_out:
					self.setBuffers_FSMPosicio(self.x_range,self.y_range,self.z_range)
					self.reset_buffer()
					self.reset_finestra()
					self.sendEvent_FSMPosicio('time_out')
			"""



if __name__ == '__main__':
	posicio = FSM_Posicio()
	mostreig = FSM_Mostrejar(posicio)
	mostreig.set_ADXL()
	mostreig.run_FSM()
			
