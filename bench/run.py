import os
import sys
import numpy as np
import json
from base64 import b32encode
from hashlib import sha1
import tkinter as tk
from tkinter import ttk
import subprocess

c = dict(
    sensor_material = "NaI",
    sensor_radius = [10, 'cm'],
    sensor_height = [20, 'cm'],

    shield_material = "Lead",
    shield_thickness = [1.0, 'cm'],

    field = [0.0, 'tesla'],
    max_step = [1.0, 'cm'],

    particle = 'e-',
    energy = [2.0, 'MeV'],
    position = [0.0, 0.0, 0.0, 'cm'],
    direction = [0, 0, 1]
)

root = tk.Tk()
status = tk.StringVar(value='Configuring')
hash_val = tk.StringVar(value='None')
sensor_val = tk.StringVar(value='NaI')
shield_val = tk.StringVar(value='Lead')
sensor_rad_val = tk.StringVar(value='10.0') # cm
sensor_hei_val = tk.StringVar(value='20.0') # cm
shield_thi_val = tk.StringVar(value='1.0') # cm
particle_val = tk.StringVar(value='e-')
energy_val = tk.StringVar(value='2.0') # MeV
n_events = tk.StringVar(value='10000') 

def open_window():

    root.geometry('700x330')
    root.title('Physics Experiment Configurator')

    s = ttk.Style()
    s.theme_use('clam')

    text = ttk.Label(text='Particle Physics Experiment on Geant4',
                     background='white', font=("Arial", 40)).grid(column=0, row=0, columnspan=2)

    text = ttk.Label(text='Detector Material',
                     background='white', font=("Arial", 20)).grid(column=0, row=1)
    combobox = ttk.Combobox(root, state="readonly", 
                            values=('NaI', 'Aerogel', 'Scintillator', 'Water', 'LiquidArgon'), 
                            textvariable=sensor_val).grid(column=1, row=1)

    text = ttk.Label(text='Shield Material',
                     background='white', font=("Arial", 20)).grid(column=0, row=2)
    combobox = ttk.Combobox(root, state="readonly", 
                            values=('Water', 'Lead', 'Mylar', 'Tungsten', 'Sus304', 'polyethylene', 'He3', 'Air', 'Galactic'), 
                            textvariable=shield_val).grid(column=1, row=2)

    text = ttk.Label(text='Particle',
                     background='white', font=("Arial", 20)).grid(column=0, row=3)
    combobox = ttk.Combobox(root, state="readonly", 
                            values=('e-', 'e+', 'mu-', 'mu+', 'neutron', 'proton', 'anti_proton', 'He3', 'alpha'), 
                            textvariable=particle_val).grid(column=1, row=3)

    text = ttk.Label(text='Particle Energy (MeV)',
                     background='white', font=("Arial", 20)).grid(column=0, row=4)
    ent = ttk.Entry(root, textvariable=energy_val, justify=tk.RIGHT).grid(column=1, row=4)

    text = ttk.Label(text='Detector Radius (cm)',
                     background='white', font=("Arial", 20)).grid(column=0, row=5)
    ent = ttk.Entry(root, textvariable=sensor_rad_val, justify=tk.RIGHT).grid(column=1, row=5)

    text = ttk.Label(text='Detector Height (cm)',
                     background='white', font=("Arial", 20)).grid(column=0, row=6)
    ent = ttk.Entry(root, textvariable=sensor_hei_val, justify=tk.RIGHT).grid(column=1, row=6)

    text = ttk.Label(text='Shield thickness (cm)',
                     background='white', font=("Arial", 20)).grid(column=0, row=7)
    ent = ttk.Entry(root, textvariable=shield_thi_val, justify=tk.RIGHT).grid(column=1, row=7)

    text = ttk.Label(text='Number of particles', justify=tk.LEFT,
                     background='white', font=("Arial", 20)).grid(column=0, row=8)
    ent = ttk.Entry(root, textvariable=n_events, justify=tk.RIGHT).grid(column=1, row=8)

    button3 = ttk.Button(root, text='Run',  command=lambda:run(c)).grid(column=0, row=9)
    button4 = ttk.Button(root, text='Exit', command=lambda:exit()).grid(column=1, row=9)
    text = ttk.Label(root, textvariable=status, background='white', font=("Arial", 20)).grid(column=0, row=10)

    root.mainloop()


def hashablize(obj):
    try:
        hash(obj)
    except TypeError:
        if isinstance(obj, dict):
            return tuple((k, hashablize(v)) for (k, v) in sorted(obj.items()))
        elif isinstance(obj, np.ndarray):
            return tuple(obj.tolist())
        elif hasattr(obj, '__iter__'):
            return tuple(hashablize(o) for o in obj)
        else:
            raise TypeError("Can't hashablize object of type %r" % type(obj))
    else:
        return obj

class NumpyJSONEncoder(json.JSONEncoder):
    """ Special json encoder for numpy types
    Edited from mpl3d: mpld3/_display.py
    """

    def default(self, obj):
        try:
            iterable = iter(obj)
        except TypeError:
            pass
        else:
            return [self.default(item) for item in iterable]
        if isinstance(obj, np.integer):
            return int(obj)
        elif isinstance(obj, np.floating):
            return float(obj)
        elif isinstance(obj, np.ndarray):
            return obj.tolist()
        return json.JSONEncoder.default(self, obj)


def deterministic_hash(thing, length=10):
    """Return a base32 lowercase string of length determined from hashing
    a container hierarchy
    """
    hashable = hashablize(thing)
    jsonned = json.dumps(hashable, cls=NumpyJSONEncoder)
    digest = sha1(jsonned.encode('ascii')).digest()
    return b32encode(digest)[:length].decode('ascii').lower()


def run(c):

    c['sensor_material'] = sensor_val.get()
    c['shield_material'] = shield_val.get()
    c['sensor_radius'][0] = float(sensor_rad_val.get())
    c['sensor_height'][0] = float(sensor_hei_val.get())
    c['shield_thickness'][0] = float(shield_thi_val.get())
    c['particle'] = particle_val.get()
    c['energy'][0] = float(energy_val.get())
    f = generate_macro(c, int(n_events.get()))
    subprocess.run(["./run.sh", f])
    subprocess.run(["mv", "out.root", f[:-4]+'.root'])

    status.set('Done, file is ' + f[:-4]+'.root')


def generate_macro(c, n=10000):

    c_hash = deterministic_hash(c)
    tmp_filename = 'config_'+c_hash+'_'+str(n)+'.mac'
    with open(tmp_filename, mode='w') as f:

        for k, v in c.items():
            if k == 'sensor_material':
                f.write('/usr/det/setMaterial ')
                f.write(v + '\n')
            if k == 'sensor_radius':
                f.write('/usr/det/setSensorRadius ')
                f.write(str(v[0]) + ' ' + v[1] + '\n')
            if k == 'sensor_height':
                f.write('/usr/det/setSensorHeight ')
                f.write(str(v[0]) + ' ' + v[1] + '\n')
            if k == 'shield_material':
                f.write('/usr/det/setShieldMaterial ')
                f.write(v + '\n')
            if k == 'shield_thickness':
                f.write('/usr/det/setShieldThickness ')
                f.write(str(v[0]) + ' ' + v[1] + '\n')
            if k == 'field':
                f.write('/usr/det/setField ')
                f.write(str(v[0]) + ' ' + v[1] + '\n')
            if k == 'max_step':
                f.write('/usr/det/setMaxStep ')
                f.write(str(v[0]) + ' ' + v[1] + '\n')
            if k == 'particle':
                f.write('/gun/particle ')
                f.write(v + '\n')
            if k == 'position':
                f.write('/gun/position ')
                f.write(str(v[0]) + ' ' + str(v[1]) + ' ' + str(v[2]) + ' ' + v[3] + '\n')
            if k == 'direction':
                f.write('/gun/direction ')
                f.write(str(v[0]) + ' ' + str(v[1]) + ' ' + str(v[2]) + '\n')
            if k == 'energy':
                f.write('/gun/energy ')
                f.write(str(v[0]) + ' ' + v[1] + '\n')
        f.write('/run/initialize'+'\n')
        f.write('/run/beamOn ' + str(int(n)))
        return tmp_filename

if __name__ == '__main__':
    open_window()
