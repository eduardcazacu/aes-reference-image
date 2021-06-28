#include "libs.h"
#include "utils.h"

int Peek(unsigned addr)
{
	void *ptr;
	unsigned page_addr, page_offset;
	unsigned page_size = sysconf(_SC_PAGESIZE);
	unsigned val;

	int _fdr = open("/dev/mem", O_RDONLY);

	if (_fdr < 0) {
		std::cerr << "Peek, cannot open mem file descriptor."  << std::endl;
		exit(-1);
	}

	page_addr = (addr & ~(page_size - 1));
	page_offset = addr - page_addr;

	ptr = mmap(NULL, page_size, PROT_READ, MAP_SHARED, _fdr, (addr & ~(page_size - 1)));

	if ((int)ptr < 0) {
		std::cerr << "Peek, memory map error." << std::endl;
		exit(-1);
	}

	val = *((unsigned *)(ptr + page_offset));

	if (munmap(ptr, page_size) < 0) {
		std::cerr << "Peek, memory unmap error." << std::endl;
		exit(-1);
	}

	if (close(_fdr) < 0) {
		std::cerr << "Peek, cannot close mem file descriptor." << std::endl;
		exit(-1);
	}

	return 0;
}

int Poke(unsigned addr, unsigned val)
{
	void *ptr;
	unsigned page_addr, page_offset;
	unsigned page_size = sysconf(_SC_PAGESIZE);

	int _fdw = open("/dev/mem", O_RDWR);

	if (_fdw < 0) {
		std::cerr << "Poke, cannot open mem file descriptor."  << std::endl;
		exit(-1);
	}

	page_addr = (addr & ~(page_size - 1));
	page_offset = addr - page_addr;

	ptr = mmap(NULL, page_size, PROT_READ | PROT_WRITE, MAP_SHARED, _fdw, (addr & ~(page_size - 1)));

	if ((int)ptr == -1) {
		std::cerr << "Poke, memory map error." << std::endl;
		exit(-1);
	}

	*((unsigned *)(ptr + page_offset)) = val;

	if (munmap(ptr, page_size) < 0) {
		std::cerr << "Poke, memory unmap error." << std::endl;
		exit(-1);
	}
	
	if (close(_fdw) < 0) {
		std::cerr << "Poke, cannot close mem file descriptor." << std::endl;
		exit(-1);
	}

	return 0;
}

int fileResponse(std::string path, utf8string* msgBody, utf8string* msgType, int* errCode)
{
	std::ifstream file;
	std::string ext, rPath = path;
    int i = path.size() - 1;

	while (path.at(i) != '.') {
		ext.insert(0, 1, path.at(i));
		i--;
	}

	if (ext == "html") {
		*msgType = "text/html";
	}
	else if (ext == "css") {
		*msgType = "text/css";
	}
    else if (ext == "js") {
		*msgType = "text/javascript";
	}
    else if (ext == "png") {
		*msgType = "image/png";
	}
	else {
		*errCode = 415;
		return -1;
	}

	rPath.erase(0, 1);
	rPath = "/var/www/" + rPath;
	file.open(rPath);

	if (file.is_open()) {
		file.seekg(0, std::ios::end);
		msgBody->reserve(file.tellg());
		file.seekg(0, std::ios::beg);

		msgBody->assign((std::istreambuf_iterator<char>(file)),	std::istreambuf_iterator<char>());
	}
	else {
		*errCode = 404;
	}

	file.close();

	return 0;
}

int AudioCodec_Read(uint8_t reg, uint16_t* read_data)
{
    uint8_t command = reg << 1;

    union i2c_smbus_data bus_data;
    struct i2c_smbus_ioctl_data args;
    args.read_write = I2C_SMBUS_READ;
    args.command = command;
    args.size = I2C_SMBUS_WORD_DATA;
    args.data = &bus_data;

    int _fdi2c = open("/dev/i2c-0", O_RDWR);

    if (_fdi2c < 0) {
		std::cerr << "AudioCodec read, cannot open i2c file descriptor."  << std::endl;
		exit(-1);
	}

    if (ioctl(_fdi2c, I2C_SLAVE, I2C_AUDIO_ADDR) < 0) {
		std::cerr << "AudioCodec read, cannot set audiocodec as i2c slave."  << std::endl;
		exit(-1);
	}

    if (ioctl(_fdi2c,I2C_SMBUS,&args) < 0) {
		std::cerr << "AudioCodec read, cannot read audiocodec."  << std::endl;
		exit(-1);
	}

    *read_data = bus_data.word;

	if (close(_fdi2c) < 0) {
		std::cerr << "AudioCodec read, cannot close i2c file descriptor."  << std::endl;
		exit(-1);
	}

    return 0;
}

int AudioCodec_Write(uint8_t reg, uint16_t write_data)
{
    uint8_t command = (reg << 1) ^ ((write_data >> 8) & 0x1);

    union i2c_smbus_data bus_data;
    struct i2c_smbus_ioctl_data args;
    args.read_write = I2C_SMBUS_WRITE;
    args.command = command;
    args.size = I2C_SMBUS_BYTE_DATA;
    args.data = &bus_data;

    bus_data.byte = (uint8_t)write_data;

    int _fdi2c = open("/dev/i2c-0", O_RDWR);

    if (_fdi2c < 0) {
		std::cerr << "AudioCodec write, cannot open i2c file descriptor."  << std::endl;
		exit(-1);
	}

    if (ioctl(_fdi2c, I2C_SLAVE, I2C_AUDIO_ADDR) < 0)  {
		std::cerr << "AudioCodec write, cannot set audiocodec as i2c slave."  << std::endl;
		exit(-1);
	}

    if (ioctl(_fdi2c,I2C_SMBUS,&args) < 0) {
		std::cerr << "AudioCodec write, cannot write audiocodec."  << std::endl;
		exit(-1);
	}

	if (close(_fdi2c) < 0) {
		std::cerr << "AudioCodec write, cannot close i2c file descriptor."  << std::endl;
		exit(-1);
	}

    return 0;
}

int AudioCodec_Default()
{
    static const uint16_t defaultVal[8] = {0x0, 0x30, 0xA, 0x79, 0x10, 0xE, 0x1, 0x20};

    AudioCodec_Write(REG15_SOFTW_RST, defaultVal[0]);
    sleep(1);
    AudioCodec_Write(REG06_POWR_MGMT, defaultVal[1]);
    AudioCodec_Write(REG00_ADC_VOL_L, defaultVal[2]);
    AudioCodec_Write(REG01_ADC_VOL_R, defaultVal[2]);
    AudioCodec_Write(REG02_DAC_VOL_L, defaultVal[3]);
    AudioCodec_Write(REG03_DAC_VOL_R, defaultVal[3]);
	AudioCodec_Write(REG04_ANAL_PATH, defaultVal[0]);
    AudioCodec_Write(REG05_DIGI_PATH, defaultVal[0]);
    AudioCodec_Write(REG07_DIGI_A_IF, defaultVal[5]);
    AudioCodec_Write(REG08_SMPL_RATE, defaultVal[0]);
    sleep(1);
    AudioCodec_Write(REG09_DIGI_ACTV, defaultVal[6]);
	sleep(1);
    AudioCodec_Write(REG06_POWR_MGMT, defaultVal[7]);
    sleep(1);
	AudioCodec_Write(REG04_ANAL_PATH, defaultVal[4]);

    return 0;
}