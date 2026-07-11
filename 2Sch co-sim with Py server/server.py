import socket
import struct
import threading

# Shared configuration constants
PORT_EVALUATE = 2
timestep = 0.00001

# --- Synchronization Mechanisms ---
# A barrier for 2 threads ensures both have received data before proceeding
sync_barrier = threading.Barrier(2)

# Shared dictionary to exchange incoming data between threads
shared_data = {}
data_lock = threading.Lock()


def handle_client_connection(port):
    """Function that runs inside its own thread to handle a specific port."""
    local_time_goal = timestep

    # Determine the "target" or opposite port for crossing the data
    other_port = 10005 if port == 10004 else 10004

    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        s.bind(("", port))
        s.listen()
        print(f"Server listening on port {port}...")

        conn, addr = s.accept()
        with conn:
            print(f"[{port}] Connected by {addr}")
            while True:
                try:
                    data = conn.recv(28)
                    if not data:
                        break
                except Exception as e:
                    print(f"[{port}] Exception: {e}")
                    break

                cmd = ((memoryview(data)[0:4]).cast("i"))[0]
                if cmd == PORT_EVALUATE:
                    inputs = (memoryview(data)[4 : len(data)]).cast("d")
                    simTime = inputs[0]
                    in1 = inputs[1]

                    # 1. Calculate the output based on THIS port's inputs
                    out1 = in1
                    
                    # 2. Save this thread's calculated data and current time_goal
                    with data_lock:
                        shared_data[port] = {
                            "time_goal": local_time_goal,
                            "out1": out1
                        }

                    try:
                        # 3. Wait here until the other thread has saved its data too
                        sync_barrier.wait()
                    except threading.BrokenBarrierError:
                        break

                    # 4. CROSSOVER: Fetch the data saved by the OTHER thread
                    with data_lock:
                        crossed_data = shared_data[other_port]

                    # 5. Pack and send the OTHER thread's data back to THIS client
                    response_data = bytearray(
                        struct.pack(
                            "ddd", 
                            crossed_data["time_goal"], 
                            crossed_data["out1"], 
                            2 * crossed_data["out1"]
                        )
                    )
                    conn.sendall(response_data)

                    # 6. Increment this thread's independent local time goal
                    local_time_goal += timestep


# --- Thread Initialization & Start ---

ports_to_serve = [10004, 10005]
threads = []

for port in ports_to_serve:
    t = threading.Thread(target=handle_client_connection, args=(port,))
    t.daemon = True
    t.start()
    threads.append(t)

print("Main thread continues to run alongside both crossed socket servers...")

# Keep main thread active
for t in threads:
    t.join()
