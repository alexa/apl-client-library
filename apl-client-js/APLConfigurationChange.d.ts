/**
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */
import { DeviceMode, IConfigurationChangeOptions, ScreenMode } from "./@types/apl-html/lib/index";
/**
 * Create and manage a configuration change payload
 */
export declare class APLConfigurationChange implements APL.ConfigurationChange {
    configurationChangePayload: IConfigurationChangeOptions;
    constructor(payload?: IConfigurationChangeOptions);
    static create(options: IConfigurationChangeOptions): APLConfigurationChange;
    size(width: number, height: number): this;
    sizeRange(width: number, minWidth: number, maxWidth: number, height: number, minHeight: number, maxHeight: number): this;
    width(width: number): this;
    height(height: number): this;
    theme(theme: string): this;
    viewportMode(mode: DeviceMode): this;
    fontScale(fontScale: number): this;
    screenMode(screenMode: ScreenMode): this;
    screenReader(enabled: boolean): this;
    disallowVideo(disallowVideo: boolean): this;
    environmentValue(key: string, value: object): this;
    mergeConfigurationChange(other: APLConfigurationChange): void;
    delete(): void;
}
