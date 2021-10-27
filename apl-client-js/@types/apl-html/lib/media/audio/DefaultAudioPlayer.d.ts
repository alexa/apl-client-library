/*
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

import { IAudioContextProvider } from './AudioContextProvider';
import { AudioPlayer } from './AudioPlayer';
import { IAudioEventListener } from './IAudioEventListener';
export declare class DefaultAudioPlayer extends AudioPlayer {
    protected contextProvider: IAudioContextProvider;
    constructor(eventListener: IAudioEventListener);
    play(id: string): void;
}
