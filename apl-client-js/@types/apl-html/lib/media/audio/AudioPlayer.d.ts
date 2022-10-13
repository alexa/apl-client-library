/*!
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */
import { IAudioEventListener } from './IAudioEventListener';
import { IAudioPlayer } from './IAudioPlayer';
export declare type IAudioNode = GainNode;
export declare abstract class AudioPlayer implements IAudioPlayer {
    private eventListener;
    private resourceMap;
    private currentSource;
    private decodePromise;
    private static logger;
    private _audioNode;
    constructor(eventListener: IAudioEventListener);
    prepare(url: string, decodeMarkers: boolean): string;
    protected onPlaybackFinished(id: string): void;
    protected onError(id: string, reason: string): void;
    abstract play(id: string): void;
    protected playWithContext(id: string, audioContext: AudioContext): void;
    protected setCurrentAudioNode(node: IAudioNode): void;
    private getConnectedAudioNode(context);
    protected disconnectCurrentAudioNode(): void;
    /**
     * Releases AudioContext. Called when destroying AudioPlayer.
     */
    abstract releaseAudioContext(): void;
    protected cancelPendingAndRemoveCompleted(): void;
    flush(): void;
}
