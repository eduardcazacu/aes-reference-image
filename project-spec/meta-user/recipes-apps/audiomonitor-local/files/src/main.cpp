#include "libs.h"
#include "utils.h"
#include "axis-fifo.h"
#include "dj_fft.h"

const char help[] = "\tfft-app <http://ip_address:port>";

web::http::experimental::listener::http_listener* fftAppListener;

struct pollfd fds;

const int pktSize = MIN_RDPKT_SIZE;
int readVal[MIN_RDPKT_SIZE];
unsigned readAvg[AVG_BUFFE_SIZE];

float magBuf[AVG_BUFFE_SIZE][HTTP_FRAM_SIZE];

int scale = 0x10;
bool mode = false;
int control = 0x40;

void handle_get(web::http::http_request request) 
{
    utf8string msgBody, msgType;
	int errCode = 0;

    fileResponse(web::uri::decode(request.relative_uri().path()), &msgBody, &msgType, &errCode);

    if (errCode == 0) {
		request.reply(web::http::status_codes::OK, msgBody, msgType);
    }
	else if (errCode == 404) {
		request.reply(web::http::status_codes::NotFound);
    }
	else if (errCode == 415) {
	    request.reply(web::http::status_codes::UnsupportedMediaType);
    }
	else {
		request.reply(web::http::status_codes::BadRequest);
    }
}

void handle_put(web::http::http_request request)
{
    web::json::value freq;

    static unsigned frequencies[HTTP_FRAM_SIZE] = {0};
    unsigned prevFrequencies[HTTP_FRAM_SIZE];

    memcpy(prevFrequencies, frequencies, sizeof(frequencies));

    for (int k = HTTP_FRAM_SIZE - 2; k >= 0; k--) {
        frequencies[k] = prevFrequencies[k+1];
        freq[U("freq" + std::to_string(k))] = web::json::value::number(prevFrequencies[k+1]);
    }

    unsigned tmp = 0;
    for (int k = 0; k < AVG_BUFFE_SIZE - 1; k++) {
        tmp = tmp + (scale * readAvg[k]);
    }
    tmp = tmp / AVG_BUFFE_SIZE;
    tmp = (unsigned)((tmp * MAX_VIEWHEIGHT * 10) / (float)MID_READ_VALUE);
    if (tmp > MAX_VIEWHEIGHT) tmp = MAX_VIEWHEIGHT;

    frequencies[15] = tmp;
    freq[U("freq15")] = web::json::value::number(tmp);

    //FFT
    for (int k = HTTP_FRAM_SIZE; k < 2*HTTP_FRAM_SIZE; k++) {
        float magBufAvg = 0;
        for (int i = 0; i < AVG_BUFFE_SIZE; i++) {
            magBufAvg = magBufAvg + magBuf[i][k-HTTP_FRAM_SIZE];
        }
        magBufAvg = magBufAvg/AVG_BUFFE_SIZE;
        magBufAvg = (magBufAvg/4000000)*MAX_VIEWHEIGHT*scale;
        if (magBufAvg > MAX_VIEWHEIGHT) magBufAvg = MAX_VIEWHEIGHT;

        freq[U("freq" + std::to_string(k))] = web::json::value::number(magBufAvg);
    }
    request.reply(web::http::status_codes::OK, freq);
}

void handle_post(web::http::http_request request)
{
    std::string path = request.relative_uri().path();
    std::string setting = path.substr(1, 4);
    unsigned value = std::stoi(path.substr(6, path.size()));

    std::cout << "Request: " << setting << " " << value << std::endl;

    if (setting == "mute") {
        if (value) {
            control = control | 0x1;
        }
        else {
            control = control & 0x7E;
        }
        Poke(CNTRL_REG_BASE, control);
    }
    else if (setting == "mode") {
        if (value) {
            mode = true;
        }
        else {
            mode = false;
        }
    }
    else if (setting == "chnl") {
        if (value) {
            control = control | 0x2;
        }
        else {
            control = control & 0x7D;
        }
        Poke(CNTRL_REG_BASE, control);
    }
    else if (setting == "dist") {
        if (value) {
            control = control | 0x20;
        }
        else {
            control = control & 0x5F;
        }
        Poke(CNTRL_REG_BASE, control);
    }
    else if (setting == "octa") {
        if (value) {
            control = control | 0x10;
        }
        else {
            control = control & 0x6F;
        }
        Poke(CNTRL_REG_BASE, control);
    }
    else if (setting == "trem") {
        if (value) {
            control = control | 0x8;
        }
        else {
            control = control & 0x77;
        }
        Poke(CNTRL_REG_BASE, control);
    }
    else if (setting == "dela") {
        if (value) {
            control = control | 0x4;
        }
        else {
            control = control & 0x7B;
        }
        Poke(CNTRL_REG_BASE, control);
    }
    else if (setting == "scle") {
        scale = value;
    }
    else if (setting == "ADCV") {
        AudioCodec_Write(REG00_ADC_VOL_L, (uint16_t)value);
        AudioCodec_Write(REG01_ADC_VOL_R, (uint16_t)value);
    }
    else if (setting == "DACV") {
        AudioCodec_Write(REG02_DAC_VOL_L, (uint16_t)value);
        AudioCodec_Write(REG03_DAC_VOL_R, (uint16_t)value);
    }
}

int initListener(char* ip)
{
    if (web::uri::validate(ip)) {
        web::http::uri fftAppIp(ip);

        fftAppListener = new web::http::experimental::listener::http_listener(fftAppIp);
        fftAppListener->open().wait();

        fftAppListener->support(web::http::methods::GET, &handle_get);
        fftAppListener->support(web::http::methods::PUT, &handle_put);
        fftAppListener->support(web::http::methods::POST, &handle_post);

        return 0;
    }

    return -1;
}

int initStreamFifo() 
{
    Poke(CNTRL_REG_BASE, 0x0);

    fds.fd = open("/dev/axis_fifo_0x43c00000", O_RDONLY);

    if (fds.fd < 0) {
        std::cerr << "Stream FIFO, cannot open file descriptor."  << std::endl;
		exit(-1);
	}

    fds.events = POLLIN | POLLRDBAND;

    if (ioctl(fds.fd, AXIS_FIFO_SET_RX_MIN_PKT, &pktSize) < 0) {
        std::cerr << "Stream FIFO read, unable to set packet size."  << std::endl;
	    exit(-1);
    }

    if (ioctl(fds.fd, AXIS_FIFO_RESET_IP) < 0) {
        std::cerr << "Stream FIFO read, cannot reset fifo."  << std::endl;
	    exit(-1);
    }

    Poke(DISTO_REG_BASE, 0x8);
    Poke(OCTAV_REG_BASE, 0x8);
    Poke(TREMO_REG_BASE, 0x8);
    Poke(DELAY_REG_BASE, 0x8);
    Poke(CNTRL_REG_BASE, control);

    return 0;
}

int main (int argc, char** argv)
{
    if (argc!=1) {
        if(!initListener(argv[1])) {
            std::cout << "Starting server at: " << argv[1] << std::endl;

            std::cout << "Initializing Audio codec.." << std::endl;
            AudioCodec_Default();

            std::cout << "Initializing Stream FIFO" << std::endl;
            initStreamFifo();

            std::cout << "Starting Stream FIFO" << std::endl;

            while(true) {
                int ret = poll(&fds, 1, -1);

                if (ret > 0) {
                    if (fds.revents & POLLIN) {
                        if (read(fds.fd, &readVal, sizeof(readVal)) < 0) {
		                    std::cerr << "Stream FIFO read, cannot read fifo."  << std::endl;
		                    exit(-1);
                        }
                        //Raw PCM values (loudness)
                        unsigned long tmp = 0;
                        unsigned prevAvg[AVG_BUFFE_SIZE];

                        memcpy(prevAvg, readAvg, sizeof(readAvg));

                        for (int k = AVG_BUFFE_SIZE - 2; k >= 0; k--) {
                            readAvg[k] = prevAvg[k+1];
                        }

                        for (int k = 0; k < MIN_RDPKT_SIZE - 1; k++) {
                            tmp = tmp + abs(readVal[k]);
                        }
                        tmp = tmp / MIN_RDPKT_SIZE;
                        readAvg[AVG_BUFFE_SIZE - 1] = tmp;
                        //FFT values
                        auto audData = std::vector<std::complex<float>>(MIN_RDPKT_SIZE);
                        float magPrevBuf[AVG_BUFFE_SIZE][HTTP_FRAM_SIZE];
                        float magAvg[HTTP_FRAM_SIZE];
                        double mag = 0;

                        for (int k = 0; k < AVG_BUFFE_SIZE; k++) {
                            memcpy(magPrevBuf[k], magBuf[k], sizeof(magAvg));
                        }   

                        for (int k = 0; k < AVG_BUFFE_SIZE - 1; k++) {
                            memcpy(magBuf[k], magPrevBuf[k + 1], sizeof(magAvg));
                        }   

                        for (int k = 0; k < MIN_RDPKT_SIZE; k++) {
                            audData[k] = (float)readVal[k];
                        }

                        auto fftData = dj::fft1d(audData, dj::fft_dir::DIR_FWD);

                        magAvg[0] = 0.2*std::abs(fftData[0]);
                        magAvg[1] = 0.3*std::abs(fftData[1]);
                        magAvg[2] = 0.4*std::abs(fftData[2]);

                        for (int k = 0; k < 2; k++) {
                            mag = mag + std::abs(fftData[3+k]);
                        }
                        magAvg[3] = 0.6*(float)(mag/2);

                        mag = 0;
                        for (int k = 0; k < 3; k++) {
                            mag = mag + std::abs(fftData[5+k]);
                        }
                        magAvg[4] = 0.8*(float)(mag/3);
                        
                        mag = 0;
                        for (int k = 0; k < 4; k++) {
                            mag = mag + std::abs(fftData[8+k]);
                        }
                        magAvg[5] = (float)(mag/4);
                        
                        mag = 0;
                        for (int k = 0; k < 5; k++) {
                            mag = mag + std::abs(fftData[12+k]);
                        }
                        magAvg[6] = 1.0*(float)(mag/5);

                        mag = 0;
                        for (int k = 0; k < 6; k++) {
                            mag = mag + std::abs(fftData[17+k]);
                        }
                        magAvg[7] = 1.2*(float)(mag/6);

                        mag = 0;
                        for (int k = 0; k < 9; k++) {
                            mag = mag + std::abs(fftData[23+k]);
                        }
                        magAvg[8] = 1.4*(float)(mag/9);

                        mag = 0;
                        for (int k = 0; k < 12; k++) {
                            mag = mag + std::abs(fftData[32+k]);
                        }
                        magAvg[9] = 1.6*(float)(mag/12);

                        mag = 0;
                        for (int k = 0; k < 16; k++) {
                            mag = mag + std::abs(fftData[44+k]);
                        }
                        magAvg[10] = 1.8*(float)(mag/16);

                        mag = 0;
                        for (int k = 0; k < 21; k++) {
                            mag = mag + std::abs(fftData[60+k]);
                        }
                        magAvg[11] = 2*(float)(mag/21);

                        mag = 0;
                        for (int k = 0; k < 27; k++) {
                            mag = mag + std::abs(fftData[81+k]);
                        }
                        magAvg[12] = 2.2*(float)(mag/27);

                        mag = 0;
                        for (int k = 0; k < 36; k++) {
                            mag = mag + std::abs(fftData[108+k]);
                        }
                        magAvg[13] = 2.4*(float)(mag/36);

                        mag = 0;
                        for (int k = 0; k < 48; k++) {
                            mag = mag + std::abs(fftData[144+k]);
                        }
                        magAvg[14] = 2.6*(float)(mag/48);

                        mag = 0;
                        for (int k = 0; k < 64; k++) {
                            mag = mag + std::abs(fftData[192+k]);
                        }
                        magAvg[15] = 2.8*(float)(mag/64);

                        memcpy(magBuf[AVG_BUFFE_SIZE - 1], magAvg, sizeof(magAvg));
                    }
                }
            }
        }
        else {
            std::cout << "Please enter a valid uri" << std::endl;
            std::cout << help << std::endl;
        }
    }
    else {
        std::cout << "Please enter a valid argument" << std::endl;
        std::cout << help << std::endl;
    }
	return 0;
}