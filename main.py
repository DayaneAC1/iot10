import socket
import time

HOST = ["192.168.1.177", 
        "192.168.1.11", 
        "192.168.1.10", 
        "192.168.1.157", 
        "192.168.1.167",
        "192.168.1.105",
        "192.168.1.174",
        "192.168.1.106",
        "192.168.1.178",
        "192.168.1.12"
        ]
PORT =  5000

MAX_TENTATIVAS = 30
TIMEOUT = 10

while True:
    ip = int(input("Escolha de 1 à 10: "))
    opcao = input("Escolha 1 para ligar ou 0 para desligar o LED, ou distancia para ultrassom: ")
    
    if ip > 10 :
        print("Dispositivo Invalido! Programa Encerrado")
        break
    elif opcao == "1":
        msg = b"ligar\n"
    elif opcao == "0":
        msg = b"desligar\n"
    elif opcao.lower() == "distancia":
        msg = b"distancia\n"
    else:
        print("Opção Invalida! Programa Encerrado")
        break
    
    s= None
    for tentativa in range(1, MAX_TENTATIVAS + 1):
        try:
            print(f"Tentando abrir conexão com ip: {HOST[ip-1]}")
            s = socket.create_connection((HOST[ip-1], PORT), timeout=2)
            print("Conexão aberta")
            s.settimeout(TIMEOUT)
            s.sendall(msg)         # ou b"ligar\n"
            print(f"Comando executado: {s.recv(16).decode()}")
            break
        except socket.timeout:
            print(f"Tempo esgotado (tentativa {tentativa}/{MAX_TENTATIVAS}).")
        except OSError as e:
            print(f"Falha de rede ({e}) (tentativa {tentativa}/{MAX_TENTATIVAS}).")

        finally:
            if s:                    # fecha só se o socket existe
                s.close()
                print("Conexão fechada.\n")