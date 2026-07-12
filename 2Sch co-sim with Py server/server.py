import socket
import struct
import sys
import threading

# Shared configuration constants
LISTENING_PORT = 10004
timestep = 0.00001

# --- Scalability Configuration ---
EXPECTED_CLIENTS = 2


sync_barrier = threading.Barrier(EXPECTED_CLIENTS)
shared_data = {}
data_lock = threading.Lock()


def handle_client_connection(conn, addr):
    """Handles a client connection, and self-terminates the script if any client drops."""
    local_time_goal = timestep
    client_id = None

    print(f"[Socket Server] Handling new raw connection from {addr}")

    try:
        with conn:
            while True:
                try:
                    data = conn.recv(28)
                    if not data:
                        print(f"\n[DISCONNECT] Client {client_id if client_id else addr} dropped connection (Clean Close).")
                        break
                except Exception as e:
                    print(f"\n[CRASH] Client {client_id if client_id else addr} dropped with exception: {e}")
                    break

                # Extract Client ID
                incoming_id = ((memoryview(data)[0:4]).cast("i"))[0]

                if client_id is None:
                    client_id = incoming_id
                    print(f"[{addr}] Identified successfully as Client {client_id}.")

                # 1. Calculate output
                inputs = (memoryview(data)[4 : len(data)]).cast("d")
                simTime = inputs[0]
                in1 = inputs[1]
                out1 = in1
                
                # 2. Save data
                with data_lock:
                    shared_data[client_id] = {
                        "time_goal": local_time_goal,
                        "out1": out1
                    }

                try:
                    # 3. MULTI-CLIENT BARRIER
                    sync_barrier.wait()
                except threading.BrokenBarrierError:
                    print(f"[System Shutdown] Client {client_id} exiting due to a peer disconnecting.")
                    return  # Instantly drop out of this thread execution

                # 4. MULTI-CLIENT CROSSOVER
                sum_other_out1 = 0.0
                other_count = 0

                with data_lock:
                    for oid, odata in shared_data.items():
                        if oid != client_id:
                            sum_other_out1 += odata["out1"]
                            other_count += 1

                avg_other_out1 = (sum_other_out1 / other_count) if other_count > 0 else 0.0

                # 5. Pack and send back
                response_data = bytearray(
                    struct.pack(
                        "ddd", 
                        local_time_goal, 
                        avg_other_out1, 
                        2 * avg_other_out1
                    )
                )
                conn.sendall(response_data)

                # 6. Increment independent local time goal
                local_time_goal += timestep

    finally:
        # --- SHUTDOWN TRIGGER ---
        # This block runs whenever a thread finishes its connection loop (either by break or exception)
        print(f"[Cleanup] Terminating session for Client {client_id if client_id else addr}...")
        
        with data_lock:
            if client_id is not None:
                shared_data.pop(client_id, None)
        
        # Resetting the barrier instantly forces a BrokenBarrierError 
        # on ALL other threads currently waiting on it.
        if not sync_barrier.broken:
            sync_barrier.reset()


# --- Dynamic Server Initialization ---

def start_server():
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        s.bind(("", LISTENING_PORT))
        s.listen(EXPECTED_CLIENTS)
        print(f"Server listening on SINGLE port {LISTENING_PORT}...")
        print(f"Awaiting EXACTLY {EXPECTED_CLIENTS} clients before starting execution loop...")

        threads = []
        for i in range(EXPECTED_CLIENTS):
            conn, addr = s.accept()
            print(f"[Progress] Client {i+1}/{EXPECTED_CLIENTS} connected.")
            t = threading.Thread(target=handle_client_connection, args=(conn, addr))
            t.daemon = True
            t.start()
            threads.append(t)

        print(f"\nAll {EXPECTED_CLIENTS} clients connected successfully! Starting simulation loop.\n")
        
        # Block the main thread until the worker threads exit.
        for t in threads:
            t.join()
            
        print("\nAll threads joined. Server script terminating safely.")

if __name__ == "__main__":
    start_server()
