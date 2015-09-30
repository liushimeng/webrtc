#include "AacDefines.h"
#include "AacVisualOnEncoder.h"

AacVisualOnEncoder::AacVisualOnEncoder()
{

}

AacVisualOnEncoder::~AacVisualOnEncoder()
{

}

bool AacVisualOnEncoder::Init()
{
    voGetAACEncAPI(&mCodecApi);

    mMemOperator.Alloc = cmnMemAlloc;
    mMemOperator.Copy = cmnMemCopy;
    mMemOperator.Free = cmnMemFree;
    mMemOperator.Set = cmnMemSet;
    mMemOperator.Check = cmnMemCheck;

    mUserData.memflag = VO_IMF_USERMEMOPERATOR;
    mUserData.memData = &mMemOperator;

    mCodecApi.Init(&mHandle, VO_AUDIO_CodingAAC, &mUserData);

    AACENC_PARAM params = { 0 };
    params.sampleRate = DEFAULT_SAMPLE_RATE;
    params.bitRate = DEFAULT_BITRATE;
    params.nChannels = MONO_NUM_CHANNELS;

    if(mCodecApi.SetParam(mHandle, VO_PID_AAC_ENCPARAM, &params) != VO_ERR_NONE)
    {
        return false;
    }

    return true;
}

int AacVisualOnEncoder::Encode(short* audioIn,
                               int inLen,
                               uint8_t* audioOut)
{
    if(audioIn == nullptr)
    {
        return 0;
    }

    int len_in_bytes = inLen * sizeof(short);

    VO_CODECBUFFER input_buffer;
    input_buffer.Length = len_in_bytes;
    input_buffer.Buffer = (uint8_t*) audioIn;

    mCodecApi.SetInputData(mHandle, &input_buffer);

    VO_CODECBUFFER output_buffer;
    output_buffer.Buffer = audioOut;
    output_buffer.Length = NUM_PCM_SAMPLES * MONO_NUM_CHANNELS;
    VO_AUDIO_OUTPUTINFO output_info;

    mCodecApi.GetOutputData(mHandle, &output_buffer, &output_info);
    return -1;
}

void AacVisualOnEncoder::CleanUp()
{
    mCodecApi.Uninit(mHandle);
}

unsigned char* AacVisualOnEncoder::getSpecificInfo()
{
    return nullptr;
}

unsigned long  AacVisualOnEncoder::getSpecificInfoLen()
{
    return -1;
}
