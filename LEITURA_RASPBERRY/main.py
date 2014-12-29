# To change this license header, choose License Headers in Project Properties.
# To change this template file, choose Tools | Templates
# and open the template in the editor.

__author__="andrei"
__date__ ="$27/10/2014 08:21:22$"




import serial
porta = '/dev/ttyACM0'
baud_rate = 9600


def ler_porta():
    valor = [0]
    try:        
        valor = arduino.readline()      
    
    except:
        print "ERRO"
        
    return valor



if __name__ == "__main__":
    #cria um objeto do tipo serial,
    arduino = serial.Serial(porta, baud_rate)
    

    leitura = ler_porta() 
       
    files = ["A.CSV","B.CSV","C.CSV","D.CSV","E.CSV","F.CSV"]
    for f in files:
        file = open(f,'w')
        file.write("date,close\n")
        file.close;
    from time import localtime,strftime
        
    while (arduino.isOpen()):
        tempo=  strftime("%H:%M:%S" ,localtime())      
        tempo=  strftime("%a %d %b %H:%M:%S" ,localtime())      
        leitura = ler_porta().replace("\n", "")
        
        if "periodo" in leitura :
            periodo = leitura.split(":")[1]
            print "periodo de amostragem: ", periodo
        
        data = leitura.split(",")
       
        
        
        for p in range(len(data)):             
            valor = data[p]
            if not "periodo" in valor: 
                if not "ok" in valor:
                    file = open(files[p], 'a')
                    if p == 0 in range(len(data)):
                        temperatura = float(valor) +273
                        print "horario:",tempo
                        print "temperatura (barometro):",temperatura , "[K]"
                        escrita = tempo+","+ str(temperatura)+"\n"
                    if p == 1 in range(len(data)):
                        pressao = (float(valor)/1000.0)
                        print "pressao:", pressao, "[kpa]"                        
                        escrita = tempo+","+str(pressao)+"\n"
                    if p == 2 in range(len(data)):
                        altitude = valor
                        print "altitude:",altitude ,"[m]"
                        escrita = tempo+","+ altitude+"\n"
                    if p == 3 in range(len(data)):
                        altitudeReal =valor
                        print "altitude Real:",altitudeReal, "[m]"
                        escrita = tempo+","+altitudeReal+"\n"
                    if p == 4 in range(len(data)):
                        temperaturadht = float(valor) + 273
                        print "temperatura (DHT):",(temperaturadht),"[K]"
                        escrita = tempo+","+str(temperaturadht)+"\n"
                    if p == 5 in range(len(data)):
                        umidade = float(float(valor)/100.0)
                        print "umidade relativa:", umidade
                        escrita = tempo+","+str(umidade)+"\n"
                    
                        print "\n"
                    
                    
                    file.write(escrita)
#                    print escrita.replace("\n","") + " " +"\n"
                    file.close()
        
        
    
    
    
