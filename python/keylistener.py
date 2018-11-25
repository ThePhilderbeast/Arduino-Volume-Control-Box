from pynput import keyboard
import subprocess
import re

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

def getMusicSinkInput():
    pattern = re.compile(r'Sink Input #(\d+).*application\.name = "([a-zA-Z]+)"')
    process = subprocess.run("pactl list sink-inputs", shell=True, stdout=subprocess.PIPE)
    stdout = "{}".format(process.stdout)
    stdout = stdout.replace(r"\n", "")
    apps = re.findall(pattern, stdout)
    matches = []
    for m in apps:
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