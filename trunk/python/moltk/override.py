
def override(interface_class):
    """
    To get Java-like derived class method override annotation.
    Courtesy of mkorpela's answer at 
    http://stackoverflow.com/questions/1167617/in-python-how-do-i-indicate-im-overriding-a-method
    """
    def override(method):
        assert(method.__name__ in dir(interface_class))
        return method
    return override
