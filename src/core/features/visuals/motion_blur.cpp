#include "dependencies/utilities/csgo.hpp"
#include "core/features/features.hpp"
#include "core/menu/variables.hpp"

void draw_screen_effect(i_material* material) {
    static auto fn = utilities::pattern_scan("client.dll", "55 8B EC 83 E4 ? 83 EC ? 53 56 57 8D 44 24 ? 89 4C 24 ?");
    int w, h;
    interfaces::engine->get_screen_size(w, h);
    __asm {
        push h
        push w
        push 0
        xor edx, edx
        mov ecx, material
        call fn
        add esp, 12
    }
}

struct MotionBlur {
    bool enabled{ false };
    bool forwardEnabled{ false };
    float fallingMin{ 10.0f };
    float fallingMax{ 20.0f };
    float fallingIntensity{ 1.0f };
    float rotationIntensity{ 1.0f };
    float strength{ 1.0f };
} motionBlur;

struct MotionBlurHistory {
    MotionBlurHistory() noexcept {
        lastTimeUpdate = 0.0f;
        previousPitch = 0.0f;
        previousYaw = 0.0f;
        previousPositon = vec3_t{ 0.0f, 0.0f, 0.0f };
        noRotationalMotionBlurUntil = 0.0f;
    }

    float lastTimeUpdate;
    float previousPitch;
    float previousYaw;
    vec3_t previousPositon;
    float noRotationalMotionBlurUntil;
};

void visuals::motion_blur(view_setup_t* setup) noexcept {
    if (!motionBlur.enabled) return;

    static MotionBlurHistory history;
    static float motionBlurValues[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
    if (setup) {
        const float timeElapsed = interfaces::globals->realtime - history.lastTimeUpdate;

        const auto viewangles = setup->angles;

        float currentPitch = viewangles.x;

        while (currentPitch > 180.0f)
            currentPitch -= 360.0f;
        while (currentPitch < -180.0f)
            currentPitch += 360.0f;

        float currentYaw = viewangles.y;

        while (currentYaw > 180.0f)
            currentYaw -= 360.0f;
        while (currentYaw < -180.0f)
            currentYaw += 360.0f;

        vec3_t currentSideVector;
        vec3_t currentForwardVector;
        vec3_t currentUpVector;
        math::angle_vectors(setup->angles, &currentForwardVector, &currentSideVector, &currentUpVector);

        vec3_t currentPosition = setup->origin;
        vec3_t positionChange = history.previousPositon - currentPosition;

        if ((positionChange.length() > 30.0f) && (timeElapsed >= 0.5f)) {
            motionBlurValues[0] = 0.0f;
            motionBlurValues[1] = 0.0f;
            motionBlurValues[2] = 0.0f;
            motionBlurValues[3] = 0.0f;
        } else if (timeElapsed > (1.0f / 15.0f)) {
            motionBlurValues[0] = 0.0f;
            motionBlurValues[1] = 0.0f;
            motionBlurValues[2] = 0.0f;
            motionBlurValues[3] = 0.0f;
        } else if (positionChange.length() > 50.0f) {
            history.noRotationalMotionBlurUntil = interfaces::globals->realtime + 1.0f;
        } else {
            const float horizontalFov = setup->fov;
            const float verticalFov = (setup->aspectRatio <= 0.0f) ? (setup->fov) : (setup->fov / setup->aspectRatio);
            const float viewdotMotion = currentForwardVector.dot(positionChange);

            if (motionBlur.forwardEnabled)
                motionBlurValues[2] = viewdotMotion;

            const float sidedotMotion = currentSideVector.dot(positionChange);
            float yawdiffOriginal = history.previousYaw - currentYaw;
            if (((history.previousYaw - currentYaw > 180.0f) || (history.previousYaw - currentYaw < -180.0f)) &&
                ((history.previousYaw + currentYaw > -180.0f) && (history.previousYaw + currentYaw < 180.0f)))
                yawdiffOriginal = history.previousYaw + currentYaw;

            float yawdiffAdjusted = yawdiffOriginal + (sidedotMotion / 3.0f);

            if (yawdiffOriginal < 0.0f)
                yawdiffAdjusted = std::clamp(yawdiffAdjusted, yawdiffOriginal, 0.0f);
            else
                yawdiffAdjusted = std::clamp(yawdiffAdjusted, 0.0f, yawdiffOriginal);

            const float undampenedYaw = yawdiffAdjusted / horizontalFov;
            motionBlurValues[0] = undampenedYaw * (1.0f - (fabsf(currentPitch) / 90.0f));

            const float pitchCompensateMask = 1.0f - ((1.0f - fabsf(currentForwardVector[2])) * (1.0f - fabsf(currentForwardVector[2])));
            const float pitchdiffOriginal = history.previousPitch - currentPitch;
            float pitchdiffAdjusted = pitchdiffOriginal;

            if (currentPitch > 0.0f)
                pitchdiffAdjusted = pitchdiffOriginal - ((viewdotMotion / 2.0f) * pitchCompensateMask);
            else
                pitchdiffAdjusted = pitchdiffOriginal + ((viewdotMotion / 2.0f) * pitchCompensateMask);


            if (pitchdiffOriginal < 0.0f)
                pitchdiffAdjusted = std::clamp(pitchdiffAdjusted, pitchdiffOriginal, 0.0f);
            else
                pitchdiffAdjusted = std::clamp(pitchdiffAdjusted, 0.0f, pitchdiffOriginal);

            motionBlurValues[1] = pitchdiffAdjusted / verticalFov;
            motionBlurValues[3] = undampenedYaw;
            motionBlurValues[3] *= (fabs(currentPitch) / 90.0f) * (fabs(currentPitch) / 90.0f) * (fabs(currentPitch) / 90.0f);

            if (timeElapsed > 0.0f)
                motionBlurValues[2] /= timeElapsed * 30.0f;
            else
                motionBlurValues[2] = 0.0f;

            motionBlurValues[2] = std::clamp((fabsf(motionBlurValues[2]) - motionBlur.fallingMin ) / (motionBlur.fallingMax - motionBlur.fallingMin), 0.0f, 1.0f) * (motionBlurValues[2] >= 0.0f ? 1.0f : -1.0f);
            motionBlurValues[2] /= 30.0f;
            motionBlurValues[0] *= motionBlur.rotationIntensity * .15f * motionBlur.strength;
            motionBlurValues[1] *= motionBlur.rotationIntensity * .15f * motionBlur.strength;
            motionBlurValues[2] *= motionBlur.rotationIntensity * .15f * motionBlur.strength;
            motionBlurValues[3] *= motionBlur.fallingIntensity * .15f * motionBlur.strength;

        }

        if (interfaces::globals->realtime < history.noRotationalMotionBlurUntil) {
            motionBlurValues[0] = 0.0f;
            motionBlurValues[1] = 0.0f;
            motionBlurValues[3] = 0.0f;
        } else {
            history.noRotationalMotionBlurUntil = 0.0f;
        }
        history.previousPositon = currentPosition;

        history.previousPitch = currentPitch;
        history.previousYaw = currentYaw;
        history.lastTimeUpdate = interfaces::globals->realtime;
        return;
    }

    const auto material = interfaces::material_system->find_material("dev/motion_blur", "RenderTargets", false);
    if (!material) return;

    const auto MotionBlurInternal = material->find_var("$MotionBlurInternal", nullptr, false);
    
    MotionBlurInternal->set_vec_component_value(motionBlurValues[0], 0);
    MotionBlurInternal->set_vec_component_value(motionBlurValues[1], 1);
    MotionBlurInternal->set_vec_component_value(motionBlurValues[2], 2);
    MotionBlurInternal->set_vec_component_value(motionBlurValues[3], 3);

    const auto MotionBlurViewPortInternal = material->find_var("$MotionBlurViewportInternal", nullptr, false);

    MotionBlurViewPortInternal->set_vec_component_value(0.0f, 0);
    MotionBlurViewPortInternal->set_vec_component_value(0.0f, 1);
    MotionBlurViewPortInternal->set_vec_component_value(1.0f, 2);
    MotionBlurViewPortInternal->set_vec_component_value(1.0f, 3);

    draw_screen_effect(material);
}