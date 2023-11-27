/**
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */
/**
 * This is a singleton class that controls the cue style in APL vidoes
 * The settings will persist between APL Renderer sessions.
 */
export declare class CueControl {
    private style;
    private showing;
    private textColor;
    private textOpacity;
    private fontSize;
    private fontFamily;
    private fontStyle;
    private backGroundColor;
    constructor();
    private _update();
    /**
     * Reset the cue style to default
     */
    reset(): void;
    /**
     * Set the cue text color
     */
    setColor(color: string): void;
    /**
     * Set the cue text opacity
     */
    setOpacity(opacity: number): void;
    /**
     * Set the cue font size
     */
    setFontSize(size: string): void;
    /**
     * Set the cue font family
     */
    setFontFamily(family: string): void;
    /**
     * Set the cue font style
     */
    setFontStyle(style: string): void;
    /**
     * Set the background color. Note: this is not supported by Chromium
     */
    setBackgroundColor(color: string): void;
    /**
     * Make all cues in APL videos visible.
     */
    show(): void;
    /**
     * Hide all cues in APL videos.
     */
    hide(): void;
    isShowing(): boolean;
    private _hideCue(video);
    private _showCue(video);
}
export declare const cueControl: CueControl;
