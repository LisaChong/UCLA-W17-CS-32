class File {
public:
	File(string name)
		: m_name(name) {};
	virtual string name() const {
		return m_name;
	};
	virtual void open() const = 0;
	virtual void redisplay() const = 0;
	virtual ~File() {};
private:
	string m_name;
};

class TextMsg : public File {
public:
	TextMsg(string name) 
		: File(name) {};
	virtual void open() const {
		cout << "open text message";
	};
	virtual void redisplay() const {
		cout << "refresh the screen";
	};
	virtual ~TextMsg() {
		cout << "Destroying " << name() << ", a text message" << endl;
	}
};

class Video : public File {
public:
	Video(string name, int length)
		: File(name), m_length(length) {};
	virtual void open() const {
		cout << "play " << m_length << " second video";
	};
	virtual void redisplay() const {
		cout << "replay video";
	};
	~Video() {
		cout << "Destroying " << name() << ", a video" << endl;
	}
private:
	int m_length;
};

class Picture : public File {
public:
	Picture(string name)
		: File(name) {};
	virtual void open() const {
		cout << "show picture";
	};
	virtual void redisplay() const {
		cout << "refresh the screen";
	};
	~Picture() {
		cout << "Destroying the picture " << name() << endl;
	}
};