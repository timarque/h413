import subprocess
from multiprocessing import Pool
import datetime

# subprocess.run(["ls", "-l"]) 
def run_command(command):
    now = datetime.datetime.now()
    print(command)
    with open(f"out/out_cmd_{command.replace(' ', '-').replace('./', '').replace('/', '--')}.txt", "a") as f:
        subprocess.call(command, shell=True, stdout=f, stderr=f)
    print(f"{command}, time: {datetime.datetime.now() - now}")


if __name__ == '__main__':
    # start 4 worker processes
    PROGRAM = "./lop"
    INSTANCES_DIR = "instances/"
    
    instance_names = open("best_known/best_known.dat").readlines()
    instance_names = [name.split()[0] for name in instance_names]

    test_commands = []

    for name in instance_names:
        test_commands.extend([
            f"{PROGRAM} -i {INSTANCES_DIR}{name} sa first insertion cw",
        ])

        test_commands.extend([
            f"{PROGRAM} -i {INSTANCES_DIR}{name} ils first insertion cw exchange 5", 
        ])
        
    
    
    now = datetime.datetime.now()
    # print(test_commands)
    with Pool(processes=3) as pool:
        pool.map(run_command, test_commands, chunksize=1)
    print(f"done, time: {datetime.datetime.now() - now}")
