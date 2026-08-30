#!/usr/bin/env zsh

cd external
mv entt/single_include/entt ./entt_single_inc
mv spdlog/include/spdlog ./spdlog_single_inc
rm -rf entt
rm -rf spdlog
mv entt_single_inc entt
mv spdlog_single_inc spdlog
cd ..
