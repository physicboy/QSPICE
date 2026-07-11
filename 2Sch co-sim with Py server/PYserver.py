import socket
import threading

HOST = '127.0.0.1'  # Localhost
PORTS = [10004, 10005]  # The list of ports you want to listen on

def handle_client(client_socket, client_address, port_listened):
    """
    Handles individual client communication.
    """
    print(f"[NEW CONNECTION] {client_address} connected to Port {port_listened}.")
    
    try:
        while True:
            data = client_socket.recv(1024)
            if not data:
                break
                
            message = data.decode('utf-8')
            print(f"[Port {port_listened}] [{client_address}]: {message}")
            
            # Echo response back, noting which port handled it
            response = f"Server (Port {port_listened}) received: {message}"
            client_socket.sendall(response.encode('utf-8'))
            
    except ConnectionResetError:
        print(f"[WARNING] Connection forcibly closed by {client_address} on Port {port_listened}")
    finally:
        print(f"[DISCONNECTED] {client_address} left Port {port_listened}.")
        client_socket.close()


def listen_on_port(port):
    """
    Runs in its own thread to accept incoming client connections for a specific port.
    """
    server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    
    try:
        server.bind((HOST, port))
        server.listen()
        print(f"[LISTENING] Server is actively monitoring port {port}...")
        
        while True:
            # This blocks, but only for this specific port's thread
            client_socket, client_address = server.accept()
            
            # Spin up a sub-thread to handle the connected client
            client_thread = threading.Thread(
                target=handle_client, 
                args=(client_socket, client_address, port),
                daemon=True  # Allows thread to exit when main program closes
            )
            client_thread.start()
            
    except Exception as e:
        print(f"[ERROR] Exception on port {port}: {e}")
    finally:
        server.close()


def start_multi_port_server():
    print("[STARTING] Initializing multi-port server setup...")
    threads = []
    
    # Create and start a listening thread for each port
    for port in PORTS:
        port_thread = threading.Thread(target=listen_on_port, args=(port,), daemon=True)
        threads.append(port_thread)
        port_thread.start()
        
    print(f"[READY] Server is running. Press Ctrl+C to stop.")
    
    # Keep the main thread alive while background threads run
    try:
        while True:
            pass
    except KeyboardInterrupt:
        print("\n[SHUTTING DOWN] Server is stopping.")

if __name__ == "__main__":
    start_multi_port_server()
