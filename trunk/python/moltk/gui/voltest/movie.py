
class Movie(list):
    def __init__(self):
        list.__init__(self)
        self.current_key_frame_index = None
        
    def append(self, key_frame):
        list.append(self, key_frame)
        self.current_key_frame_index = len(self) - 1
        
    def increment(self):
        if len(self) < 1:
            self.current_key_frame_index = None
            return
        self.current_key_frame_index += 1
        if (self.current_key_frame_index) >= len(self):
            self.current_key_frame_index = 0;
        
    def decrement(self):
        if len(self) < 1:
            self.current_key_frame_index = None
            return
        self.current_key_frame_index -= 1
        if (self.current_key_frame_index) < 0:
            self.current_key_frame_index = len(self) - 1;

    def clear(self):
        self.current_key_frame_index = None
        empty = []
        self[:] = empty[:]

    @property
    def current_key_frame(self):
        if self.current_key_frame_index is None:
            return None
        return self[self.current_key_frame_index]

class KeyFrame:
    def __init__(self, camera_state):
        self.camera_state = camera_state
