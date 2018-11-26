#! /usr/bin/python2

from pynput import keyboard
import subprocess
import re

musicApps = ["Spotify"]
pattern = re.compile(r'(?!Sink Input #.*?module-loopback)Sink Input #(\d+).*?application\.name = "([a-zA-Z]+)"')


def on_press(key):
    # mic vol up and down
    if (key == keyboard.Key.f13):
        subprocess.run("pactl set-source-mute alsa_input.usb-Samson_Technologies_Samson_C01U-00.analog-stereo toggle", shell=True)
    if (key == keyboard.Key.f14):
        subprocess.run("pactl set-source-volume alsa_input.usb-Samson_Technologies_Samson_C01U-00.analog-stereo +3%", shell=True)
    if (key == keyboard.Key.f15):
        subprocess.run("pactl set-source-volume alsa_input.usb-Samson_Technologies_Samson_C01U-00.analog-stereo -3%", shell=True)

    # music vol
    if (key == keyboard.Key.f16):
        for m in getMusicSinkInput():
            subprocess.run("pactl set-sink-input-volume {} +3%".format(m), shell=True)
    if (key == keyboard.Key.f17):
        for m in getMusicSinkInput():
            subprocess.run("pactl set-sink-input-volume {} -3%".format(m), shell=True)

    # main Vol
    if (key == keyboard.Key.f20):
        subprocess.run("pactl set-sink-mute alsa_output.pci-0000_00_1f.3.analog-stereo toggle", shell=True)
    if (key == keyboard.Key.f18):
        subprocess.run("pactl set-sink-volume alsa_output.pci-0000_00_1f.3.analog-stereo +3%", shell=True)
    if (key == keyboard.Key.f19):
        subprocess.run("pactl set-sink-volume alsa_output.pci-0000_00_1f.3.analog-stereo -3%", shell=True)

def getMusicSinkInput():
    process = subprocess.run("pactl list sink-inputs", shell=True, stdout=subprocess.PIPE)
    stdout = "{}".format(process.stdout)
    stdout = stdout.replace(r"\n", "")
    apps = re.findall(pattern, stdout)
    matches = []
    for m in apps:
        # print("index: {} app:{}".format(m[0], m[1]))
        if (m[1] in musicApps):
            matches.append(m[0])
    return matches

def on_release(key):
    if key == keyboard.Key.esc:
        return True
        # return False

# Collect events until released
with keyboard.Listener(
        on_press=on_press,
        on_release=on_release) as listener:
    listener.join()