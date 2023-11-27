/**
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */
declare namespace APL {
    export class MediaPlayerFactory extends Deletable {
        public static create(playerFactory): MediaPlayerFactory;
        public destroy(): void;
    }
}
