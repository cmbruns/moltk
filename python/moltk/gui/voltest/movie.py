from interpolate import CatmullRomSpline
import camera
from rotation import Vec3
from PySide.QtCore import QElapsedTimer, QCoreApplication

class Movie():
    def __init__(self, frames_per_second=24.0):
        self._key_frames = []
        # Create value arrays for convenient interpolation
        self._focusx = []
        self._focusy = []
        self._focusz = []
        self._zfv = []
        self.current_key_frame_index = None
        self.frames_per_second = frames_per_second
        self.spline = CatmullRomSpline()
        
    def __len__(self):
        return len(self._key_frames)
    
    def append(self, key_frame):
        self._key_frames.append(key_frame)
        self._focusx.append(key_frame.camera_state.focus.x)
        self._focusy.append(key_frame.camera_state.focus.y)
        self._focusz.append(key_frame.camera_state.focus.z)
        self._zfv.append(key_frame.camera_state.zFocus_vheight)
        self.current_key_frame_index = len(self._key_frames) - 1
        
    def increment(self):
        if len(self._key_frames) < 1:
            self.current_key_frame_index = None
            return
        self.current_key_frame_index += 1
        if (self.current_key_frame_index) >= len(self._key_frames):
            self.current_key_frame_index = 0;
        
    def decrement(self):
        if len(self._key_frames) < 1:
            self.current_key_frame_index = None
            return
        self.current_key_frame_index -= 1
        if (self.current_key_frame_index) < 0:
            self.current_key_frame_index = len(self._key_frames) - 1;

    def clear(self):
        self.current_key_frame_index = None
        self._key_frames[:] = []

    def play(self, doLoop=False):
        time = 0.0 # seconds
        timer = QElapsedTimer()
        timer.restart()
        dtime = 1.0 / self.frames_per_second
        key_frame_number = 0
        for kf in self._key_frames:
            self.current_key_frame_index = key_frame_number
            if (not doLoop) and (key_frame_number == len(self._key_frames) - 1):
                return # Don't go past final frame
            key_frame_time = 0.0
            while key_frame_time < kf.time_to_next_frame:
                tf = key_frame_time / kf.time_to_next_frame
                interpolation_parameter = tf + key_frame_number
                camera_state = camera.State()
                focus_x = self.spline.interpolate_sequence(
                            self._focusx, 
                            interpolation_parameter,
                            doLoop)
                focus_y = self.spline.interpolate_sequence(
                            self._focusy, 
                            interpolation_parameter,
                            doLoop)
                focus_z = self.spline.interpolate_sequence(
                            self._focusz, 
                            interpolation_parameter,
                            doLoop)
                camera_state.zFocus_vheight = self.spline.interpolate_sequence(
                            self._zfv,
                            interpolation_parameter,
                            doLoop)
                # TODO rotation
                camera_state.rotation = kf.camera_state.rotation
                camera_state.focus= Vec3([focus_x, focus_y, focus_z])
                while timer.elapsed() < time*1000.0:
                    # TODO - use threading instead
                    QCoreApplication.processEvents()
                yield KeyFrame(camera_state)
                key_frame_time += dtime
                time += dtime
            key_frame_number += 1
        
    @property
    def current_key_frame(self):
        if self.current_key_frame_index is None:
            return None
        return self._key_frames[self.current_key_frame_index]

class KeyFrame:
    def __init__(self, camera_state, time=2.5):
        self.camera_state = camera_state
        self.time_to_next_frame = time # seconds
