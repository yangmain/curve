/*
 * Project: curve
 * Created Date: Mon May 13 2019
 * Author: xuchaojie
 * Copyright (c) 2019 netease
 */

#include <gtest/gtest.h>

#include "src/mds/copyset/copyset_manager.h"
#include "test/mds/copyset/test_helper.h"


namespace curve {
namespace mds {
namespace copyset {

TEST(TestCopysetManager, InitSuccess) {
    CopysetOption option;
    CopysetManager manager(option);

    CopysetConstrait constrait;
    constrait.zoneNum = 3;
    constrait.zoneChoseNum = 3;
    constrait.replicaNum = 3;

    ASSERT_TRUE(manager.Init(constrait));
}

TEST(TestCopysetManager, InitFail) {
    CopysetOption option;
    CopysetManager manager(option);

    CopysetConstrait constrait;
    constrait.zoneNum = 4;
    constrait.zoneChoseNum = 4;
    constrait.replicaNum = 4;

    ASSERT_FALSE(manager.Init(constrait));
}

TEST(TestCopysetManager, GenCopysetByCopysetNumSuccess) {
    CopysetOption option;
    CopysetManager manager(option);

    CopysetConstrait constrait;
    constrait.zoneNum = 3;
    constrait.zoneChoseNum = 3;
    constrait.replicaNum = 3;

    ASSERT_TRUE(manager.Init(constrait));

    TestCluster cluster;
    cluster.SetUniformCluster();

    std::vector<Copyset> out;
    ASSERT_TRUE(manager.GenCopyset(cluster,
        3, 0, &out));
    ASSERT_EQ(3, out.size());
}

TEST(TestCopysetManager, GenCopysetByScatterWidthSuccess) {
    CopysetOption option;
    CopysetManager manager(option);

    CopysetConstrait constrait;
    constrait.zoneNum = 3;
    constrait.zoneChoseNum = 3;
    constrait.replicaNum = 3;

    ASSERT_TRUE(manager.Init(constrait));

    TestCluster cluster;
    cluster.SetUniformCluster();

    std::vector<Copyset> out;
    ASSERT_TRUE(manager.GenCopyset(cluster,
        0, 4, &out));
}

TEST(TestCopysetManager, GenCopysetByCopysetNumAndValidSuccess) {
    CopysetOption option;
    option.copysetRetryTimes = 10;
    option.scatterWidthFloatingPercentage = 20;
    CopysetManager manager(option);

    CopysetConstrait constrait;
    constrait.zoneNum = 3;
    constrait.zoneChoseNum = 3;
    constrait.replicaNum = 3;

    ASSERT_TRUE(manager.Init(constrait));

    {
        // 180个chunkserver，3个zone，6000个copyset
        TestCluster cluster;
        cluster.SetMassiveCluster(180, 3);

        std::vector<Copyset> out;
        ASSERT_TRUE(manager.GenCopyset(cluster,
            6000, 0, &out));
        ASSERT_EQ(6000, out.size());
    }

    {
        // 180个chunkserver，3个zone，9000个copyset
        TestCluster cluster;
        cluster.SetMassiveCluster(180, 3);

        std::vector<Copyset> out;
        ASSERT_TRUE(manager.GenCopyset(cluster,
            9000, 0, &out));
        ASSERT_EQ(9000, out.size());
    }

    {
        // 240个chunkserver 3个zone， 9000个copyset
        TestCluster cluster;
        cluster.SetMassiveCluster(240, 3);

        std::vector<Copyset> out;
        ASSERT_TRUE(manager.GenCopyset(cluster,
            9000, 0, &out));
        ASSERT_EQ(9000, out.size());
    }

    {
        // 240个chunkserver 3个zone， 12000个copyset
        TestCluster cluster;
        cluster.SetMassiveCluster(240, 3);

        std::vector<Copyset> out;
        ASSERT_TRUE(manager.GenCopyset(cluster,
            12000, 0, &out));
        ASSERT_EQ(12000, out.size());
    }
}

}  // namespace copyset
}  // namespace mds
}  // namespace curve