/**
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */
declare namespace APL {
    export class MediaPlayer extends Deletable {
        updateMediaState(mediaState: any): void;
        doCallback(eventType: number): void;
        getMediaPlayerHandle(): IMediaPlayerHandle;
    }
}
