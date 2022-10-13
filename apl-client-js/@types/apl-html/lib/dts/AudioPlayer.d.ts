/**
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */
declare namespace APL {
    export class AudioPlayer extends Deletable {
        public onPrepared(id: string): void;
        public onMarker(id: string, markers): void;
        public onPlaybackStarted(id: string): void;
        public onPlaybackFinished(id: string): void;
        public onError(id: string, reason: string): void;
    }
}
