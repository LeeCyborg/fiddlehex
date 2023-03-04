/*
 * Pretend your DMX lights are neopixels using DFRobot shield and 4 channel
 * lights + DMX_master.ino library
 * Lee Wilkins November 2018
 * https://github.com/LeeRobot/DMX-Pretends-To-Be-Neopixel
 */
#pragma once

#include <Arduino.h>
#include <Conceptinetics.h>
#include <FastLED.h>

/**
 * @defgroup   Arduino DMX lighting control
 * @brief      DMX lighting control
 *
 * CTC-DRA-10-R2 ISOLATED DMX-RDM SHIELD JUMPER INSTRUCTIONS
 *
 * Set !EN (right hand side) when uploading
 * Set EN (left hand side) or no jumper when running
 *
 * Set DE jumper
 * Set TX-uart jumper
 * Set RX-uart jumper
 *
 * If you are controlling lights for example:
 * Channel 1 is red first lamp
 * Channel 2 is green
 * Channel 3 is blue
 * Channel 4 is white
 *
 * Channel spacing is set to 10
 * So for second lamp:
 * Channel 11 is red
 * Channel 12 is green
 * Channel 13 is blue
 * Channel 14 is white
 *
 * Third lamp Channel 21 is red, and so on.
 *
 * @{
 *
 */

/**
 * @brief Dmx configuration status
 */
enum eArdDmxStatus
{
    /**
     * @brief Operation successful
     */
    kArdDmxSuccess = 0,

    /**
     * @brief Failed to allocate memory
     */
    kArdDmxAllocFailed,

    /**
     * @brief Wrong input parameters provided
     */
    kArdDmxBadInput
};

/**
 * @brief Order of DMX color channels
 */
enum eArdDmxOrder {
    /** @brief Order [R, G, B] */
	kArdDmxRGB,
    /** @brief Order [R, B, G] */
	kArdDmxRBG,
    /** @brief Order [G, R, B] */
	kArdDmxGRB,
    /** @brief Order [G, B, R] */
	kArdDmxGBR,
    /** @brief Order [B, R, G] */
	kArdDmxBRG,
    /** @brief Order [B, G, R] */
	kArdDmxBGR,

    /** @brief Order [R, G, B, W] */
	kArdDmxRGBW,
    /** @brief Order [R, B, G, W] */
	kArdDmxRBGW,
    /** @brief Order [G, R, B, W] */
	kArdDmxGRBW,
    /** @brief Order [G, B, R, W] */
	kArdDmxGBRW,
    /** @brief Order [B, R, G, W] */
	kArdDmxBRGW,
    /** @brief Order [B, G, R, W] */
	kArdDmxBGRW,

    /** @brief Order [W, R, G, B] */
	kArdDmxWRGB,
    /** @brief Order [W, R, B, G] */
	kArdDmxWRBG,
    /** @brief Order [W, G, R, B] */
	kArdDmxWGRB,
    /** @brief Order [W, G, B, R] */
	kArdDmxWGBR,
    /** @brief Order [W, B, R, G] */
	kArdDmxWBRG,
    /** @brief Order [W, B, G, R] */
	kArdDmxWBGR
};

/**
 * @brief Parameters for LED strip
 */
struct ArdDmxParameters
{
    /**
     * @brief Number of lights to control
     */
    uint8_t pin = 2;

    /**
     * @brief Number of lights (leds) to control
     */
    uint16_t num_leds = 0;

    /**
     * @brief Color channel order.
     */
    eArdDmxOrder color_order = kArdDmxRGB;

    /**
     * @brief Number of channels per light.
     *
     * This number must be equal to or larger than the number of channels supported.
     */
    uint16_t dmx_channels_per_light = 4;

    /**
     * @brief Offset all channel values by this amount.
     */
    uint16_t dmx_channel_offset = 0;

    /**
     * @brief Highest channel value. Leave this at 512 when using DMX512 protocol.
     */
    uint16_t dmx_max_channels = 512;
};


/**
 * @brief Dmx lights control for Arduino
 */
class ArdDmx
{
   public:
    ArdDmx() = default;

    // not assignable
    ArdDmx(ArdDmx const &) = delete;
    ArdDmx &operator=(ArdDmx const &) = delete;

    ~ArdDmx()
    {
        if (m_dmx_master != nullptr)
        {
            // clear and disable
            m_dmx_master->setChannelRange(m_params.dmx_channel_offset,
                                          m_params.dmx_max_channels, 0);
            m_dmx_master->disable();
        }
        delete m_dmx_master;
        delete m_leds;
    }

    /**
     * @brief Initialize lights
     *
     * This method must be called before any other method is to be called
     * successfully.
     *
     * @param parameters DMX configuration parameters.
     * @return eArdDmxStatus Status of initialization
     */
    eArdDmxStatus Initialize(const ArdDmxParameters &parameters);

    /**
     * @brief Get the number of pixels in strip.
     *
     * @return Number of pixels.
     */
    uint16_t GetNumPixels() const { return m_params.num_leds; }

    /**
     * @brief Get pixel color.
     *
     * @param pixel
     * @return CRGB
     */
    CRGB GetPixel(uint16_t pixel) const;

    /**
     * @brief Get pixel white value
     *
     * @param pixel
     * @return uint8_t
     */
    uint8_t GetWhitePixel(uint16_t pixel) const;

    /**
     * @brief Set pixel color
     *
     * @param pixel
     * @param color
     * @param white
     */
    void SetPixelColor(uint16_t pixel, const CRGB& color, uint8_t white = 0);

    /**
     * @brief Set pixel red, green, and blue values.
     *
     * @param pixel
     * @param red
     * @param green
     * @param blue
     * @param white
     */
    void SetPixel(uint16_t pixel, uint8_t red, uint8_t green, uint8_t blue, uint8_t white = 0);

    /**
     * @brief Set pixel hue, saturation, and value.
     *
     * @param pixel
     * @param hue
     * @param sat
     * @param value
     * @param white
     */
    void SetPixelHSV(uint16_t pixel, uint8_t hue, uint8_t sat, uint8_t value, uint8_t white = 0);

    /**
     * @brief Set all pixels to red, green, and blue values.
     *
     * @param red
     * @param green
     * @param blue
     * @param white
     */
    void SetAll(uint8_t red, uint8_t green, uint8_t blue, uint8_t white = 0);

    /**
     * @brief Show lights
     */
    void ShowStrip();

   private:

    static void SetColorOrder(eArdDmxOrder colorOrder, uint8_t rgbw[4]);

    CRGB *m_leds = nullptr;
    uint8_t *m_white_leds = nullptr;
    DMX_Master *m_dmx_master = nullptr;
    ArdDmxParameters m_params;
    uint8_t m_rgbw[4] = {1, 2, 3, 4};
};

/**
 * @}
 */

inline void ArdDmx::SetColorOrder(eArdDmxOrder colorOrder, uint8_t rgbw[4])
{
    switch (colorOrder)
    {
    case kArdDmxRGB:
    case kArdDmxRGBW:
        rgbw[0] = 1, rgbw[1] = 2, rgbw[2] = 3, rgbw[3] = 4;
        break;
	case kArdDmxGBR:
	case kArdDmxGBRW:
        rgbw[0] = 2, rgbw[1] = 3, rgbw[2] = 1, rgbw[3] = 4;
        break;
	case kArdDmxBRG:
	case kArdDmxBRGW:
        rgbw[0] = 3, rgbw[1] = 1, rgbw[2] = 2, rgbw[3] = 4;
        break;
    case kArdDmxRBG:
    case kArdDmxRBGW:
        rgbw[0] = 1, rgbw[1] = 3, rgbw[2] = 2, rgbw[3] = 4;
        break;
    case kArdDmxBGR:
    case kArdDmxBGRW:
        rgbw[0] = 3, rgbw[1] = 2, rgbw[2] = 1, rgbw[3] = 4;
        break;
    case kArdDmxGRB:
    case kArdDmxGRBW:
        rgbw[0] = 2, rgbw[1] = 1, rgbw[2] = 3, rgbw[3] = 4;
        break;
    case kArdDmxWRGB:
        rgbw[0] = 2, rgbw[1] = 3, rgbw[2] = 4, rgbw[3] = 1;
        break;
    case kArdDmxWGBR:
        rgbw[0] = 3, rgbw[1] = 4, rgbw[2] = 2, rgbw[3] = 1;
        break;
    case kArdDmxWBRG:
        rgbw[0] = 4, rgbw[1] = 2, rgbw[2] = 3, rgbw[3] = 1;
        break;
    case kArdDmxWRBG:
        rgbw[0] = 2, rgbw[1] = 4, rgbw[2] = 3, rgbw[3] = 1;
        break;
    case kArdDmxWBGR:
        rgbw[0] = 4, rgbw[1] = 3, rgbw[2] = 2, rgbw[3] = 1;
        break;
    case kArdDmxWGRB:
        rgbw[0] = 3, rgbw[1] = 2, rgbw[2] = 4, rgbw[3] = 1;
        break;
    default:
        // RGBW
        rgbw[0] = 1, rgbw[1] = 2, rgbw[2] = 3, rgbw[3] = 4;
        break;
    }
}

inline eArdDmxStatus ArdDmx::Initialize(const ArdDmxParameters &parameters)
{
    // check inputs
    if (parameters.num_leds < 1 || parameters.dmx_channels_per_light < 4)
    {
        return kArdDmxBadInput;
    }

    // Deallocate memory
    delete m_leds;
    m_leds = nullptr;
    delete m_white_leds;
    m_white_leds = nullptr;
    delete m_dmx_master;
    m_dmx_master = nullptr;

    // LEDs
    // ----

    // Allocate memory
    m_leds = (CRGB *)calloc(parameters.num_leds, sizeof(CRGB));
    if (m_leds == nullptr)
    {
        return kArdDmxAllocFailed;
    }
    m_white_leds = (uint8_t *)calloc(parameters.num_leds, sizeof(uint8_t));
    if (m_white_leds == nullptr)
    {
        return kArdDmxAllocFailed;
    }
    SetColorOrder(parameters.color_order, m_rgbw);

    // DMX Master
    // ----------

    // Initialize DMX master
    m_dmx_master = new DMX_Master(parameters.dmx_max_channels, parameters.pin);
    if (m_dmx_master == nullptr)
    {
        free(m_leds);
        m_leds = nullptr;
        return kArdDmxAllocFailed;
    }
    // enable and clear all channels
    m_dmx_master->enable();
    m_dmx_master->setChannelRange(parameters.dmx_channel_offset,
                                  parameters.dmx_max_channels, 0);

    // Save parameters
    // ---------------

    m_params = parameters;

    return kArdDmxSuccess;
}

inline uint8_t ArdDmx::GetWhitePixel(uint16_t pixel) const
{
    if (pixel < m_params.num_leds)
    {
        return m_white_leds[pixel];
    }
    else
    {
        return 0;
    }
}

inline CRGB ArdDmx::GetPixel(uint16_t pixel) const
{
    if (pixel < m_params.num_leds)
    {
        return m_leds[pixel];
    }
    else
    {
        return CRGB();
    }
}

inline void ArdDmx::SetPixelColor(uint16_t pixel, const CRGB& color, uint8_t white)
{
    if (pixel < m_params.num_leds)
    {
        m_leds[pixel] = color;
        m_white_leds[pixel] = white;
    }
}

inline void ArdDmx::SetPixel(uint16_t pixel, uint8_t red, uint8_t green, uint8_t blue, uint8_t white)
{
    if (pixel < m_params.num_leds)
    {
        m_leds[pixel].setRGB(red, green, blue);
        m_white_leds[pixel] = white;
    }
}

inline void ArdDmx::SetPixelHSV(uint16_t pixel, uint8_t hue, uint8_t sat, uint8_t val, uint8_t white)
{
    if (pixel < m_params.num_leds)
    {
        m_leds[pixel].setHSV(hue, sat, val);
        m_white_leds[pixel] = white;
    }
}

inline void ArdDmx::SetAll(uint8_t red, uint8_t green, uint8_t blue, uint8_t white)
{
    for (uint16_t pixel = 0; pixel < m_params.num_leds; ++pixel)
    {
        m_leds[pixel].setRGB(red, green, blue);
        m_white_leds[pixel] = white;
    }
}

inline void ArdDmx::ShowStrip()
{
    if (m_dmx_master != nullptr)
    {
        for (uint16_t pixel = 0; pixel < m_params.num_leds; ++pixel)
        {
            // Set color
            const uint16_t start_channel = m_params.dmx_channel_offset + m_params.dmx_channels_per_light * pixel;
            CRGB& color = m_leds[pixel];
            m_dmx_master->setChannelValue(start_channel + m_rgbw[0], color.r);
            m_dmx_master->setChannelValue(start_channel + m_rgbw[1], color.g);
            m_dmx_master->setChannelValue(start_channel + m_rgbw[2], color.b);
            m_dmx_master->setChannelValue(start_channel + m_rgbw[3], m_white_leds[pixel]);
        }
    }
}
