/*!
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */
export interface IAudioPlayer {
    prepare(url: string, decodeMarkers: boolean): string;
    play(id: string): void;
    releaseAudioContext(): void;
    flush(): void;
}
